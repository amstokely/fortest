module fortest_test_session
    use fortest_test_suite, only : test_suite_t
    use procedure_interfaces_mod, only : test_proc, fixture_proc
    use iso_c_binding
    implicit none

    public :: test_session_t

    type :: test_suite_node_t
        type(test_suite_t) :: suite
        type(test_suite_node_t), pointer :: next => null()
    end type test_suite_node_t

    type :: test_session_t
        private
        type(test_suite_node_t), pointer :: head => null()
        type(test_suite_node_t), pointer :: tail => null()

    contains
        procedure :: register_test_suite
        procedure :: get_test_suite
        procedure :: register_fixture => register_fixture
        procedure :: register_test
        procedure :: run
        procedure, public :: finalize
        procedure, public :: get_status
    end type test_session_t

contains

    !> Register a new test suite and add it to the linked list
    subroutine register_test_suite(this, test_suite_name)
        use fortest_test_suite, only : test_suite_t
        use f_c_string_t_mod, only : f_c_string_t

        class(test_session_t), intent(inout) :: this
        character(len = *), intent(in) :: test_suite_name
        type(test_suite_t) :: test_suite
        type(f_c_string_t) :: f_c_string_test_suite_name
        type(test_suite_node_t), pointer :: new_node
        integer :: status

        interface
            subroutine c_register_test_suite(name) bind(C, name = "c_register_test_suite")
                import :: c_ptr
                type(c_ptr), value :: name
            end subroutine c_register_test_suite
        end interface

        ! Initialize the new suite
        test_suite%name = test_suite_name
        f_c_string_test_suite_name = f_c_string_t(test_suite_name)
        status = f_c_string_test_suite_name%to_c()
        call c_register_test_suite(f_c_string_test_suite_name%get_c_string())

        ! Create a new linked list node
        allocate(new_node)
        new_node%suite = test_suite
        new_node%next => null()

        if (.not. associated(this%head)) then
            this%head => new_node
            this%tail => new_node
        else
            this%tail%next => new_node
            this%tail => new_node
        end if
    end subroutine register_test_suite


    subroutine register_test(this, test_suite_name, test_name, test)
        use iso_c_binding
        use f_c_string_t_mod, only : f_c_string_t
        class(test_session_t), intent(in) :: this
        character(len = *), intent(in) :: test_suite_name
        character(len = *), intent(in) :: test_name
        procedure(test_proc) :: test
        type(f_c_string_t) :: f_c_string_test_suite_name
        type(f_c_string_t) :: f_c_string_test_name
        integer :: status
        interface
            subroutine c_register_test(test_suite_name, test_name, test) bind(C, name = "c_register_test")
                import :: c_ptr, c_funptr
                type(c_ptr), value :: test_suite_name
                type(c_ptr), value :: test_name
                type(c_funptr), value :: test
            end subroutine c_register_test
        end interface

        f_c_string_test_suite_name = f_c_string_t(test_suite_name)
        status = f_c_string_test_suite_name%to_c()
        f_c_string_test_name = f_c_string_t(test_name)
        status = f_c_string_test_name%to_c()
        call c_register_test(&
                f_c_string_test_suite_name%get_c_string(), &
                f_c_string_test_name%get_c_string(), &
                c_funloc(test))
    end subroutine register_test

    subroutine register_fixture(this, setup, teardown, args, scope, test_suite_name)
        class(test_session_t), intent(in) :: this
        procedure(fixture_proc) :: setup
        procedure(fixture_proc) :: teardown
        type(c_ptr), value :: args
        character(len = *), intent(in) :: scope
        character(len = *), intent(in) :: test_suite_name
        type(test_suite_t), pointer :: test_suite_ptr

        test_suite_ptr => this%get_test_suite(test_suite_name)
        if (associated(test_suite_ptr)) then
            call test_suite_ptr%register_fixture(setup, teardown, args, scope)
        end if
    end subroutine register_fixture

    subroutine run(this)
        class(test_session_t), intent(in) :: this
        interface
            subroutine c_run_test_session() bind(C, name = "c_run_test_session")
            end subroutine c_run_test_session
        end interface

        call c_run_test_session()
    end subroutine run

    function get_status(this) result(status)
        class(test_session_t), intent(inout) :: this
        type(test_suite_node_t), pointer :: node
        integer :: status

        status = 0
        node => this%head
        do while (associated(node))
            status = status + node%suite%get_status()
            node => node%next
        end do
    end function get_status

    subroutine finalize(this)
        class(test_session_t), intent(inout) :: this
        type(test_suite_node_t), pointer :: node, temp
        integer :: status

        status = this%get_status()
        node => this%head
        do while (associated(node))
            temp => node%next
            deallocate(node)
            node => temp
        end do
        this%head => null()
        this%tail => null()
        call exit(status)
    end subroutine finalize


    !> Private: find a suite by name
    function get_test_suite(this, name) result(suite_ptr)
        class(test_session_t), intent(in) :: this
        character(len = *), intent(in) :: name
        type(test_suite_t), pointer :: suite_ptr

        type(test_suite_node_t), pointer :: node
        suite_ptr => null()

        node => this%head
        do while (associated(node))
            if (trim(node%suite%name) == trim(name)) then
                suite_ptr => node%suite
                return
            end if
            node => node%next
        end do
    end function get_test_suite

end module fortest_test_session

module procedure_interfaces_mod
    use iso_c_binding
    implicit none
    private
    public :: test_proc, fixture_proc

    abstract interface
        subroutine test_proc(t_ptr, ts_ptr, s_ptr)
            import :: c_ptr
            type(c_ptr), value :: t_ptr, ts_ptr, s_ptr
        end subroutine test_proc
    end interface

    abstract interface
        subroutine fixture_proc(args)
            import :: c_ptr
            type(c_ptr), value :: args
        end subroutine fixture_proc
    end interface

end module procedure_interfaces_mod
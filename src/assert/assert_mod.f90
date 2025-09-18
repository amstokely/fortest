module fortest_assert
   use iso_c_binding, only: c_int, c_float, c_double, c_ptr
   use f_c_string_t_mod, only: f_c_string_t

   implicit none
   private

   public :: assert_equal
   public :: assert_not_equal
   public :: assert_true
   public :: assert_false

   interface assert_equal
      module procedure assert_equal_int
      module procedure assert_equal_float
      module procedure assert_equal_double
      module procedure assert_equal_string
   end interface assert_equal

   interface assert_not_equal
      module procedure assert_not_equal_int
      module procedure assert_not_equal_float
      module procedure assert_not_equal_double
      module procedure assert_not_equal_string
   end interface assert_not_equal

contains

   subroutine assert_equal_int(expected, actual)
      implicit none
      integer(c_int) :: expected, actual
      interface
         subroutine c_assert_equal_int(expected, actual) bind(C)
            import :: c_int
            integer(c_int), value :: expected, actual
         end subroutine
      end interface
      call c_assert_equal_int(expected, actual)
   end subroutine assert_equal_int

   subroutine assert_equal_float(expected, actual)
      implicit none
      real(c_float) :: expected, actual
      interface
         subroutine c_assert_equal_float(expected, actual) bind(C)
            import :: c_float
            real(c_float), value :: expected, actual
         end subroutine
      end interface
      call c_assert_equal_float(expected, actual)
   end subroutine assert_equal_float

   subroutine assert_equal_double(expected, actual)
      implicit none
      real(c_double) :: expected, actual
      interface
         subroutine c_assert_equal_double(expected, actual) bind(C)
            import :: c_double
            real(c_double), value :: expected, actual
         end subroutine
      end interface
      call c_assert_equal_double(expected, actual)
   end subroutine assert_equal_double

   subroutine assert_equal_string(expected, actual)
      implicit none
      character(len = *) :: expected, actual
      type(f_c_string_t) :: f_c_string_expected, f_c_string_actual
      integer :: status
      interface
         subroutine c_assert_equal_string(expected, actual) bind(C)
            import :: c_ptr
            type(c_ptr), value, intent(in) :: expected, actual
         end subroutine
      end interface
      status = 0
      f_c_string_expected = f_c_string_t(expected)
      f_c_string_actual = f_c_string_t(actual)
      status = f_c_string_expected%to_c()
      status = f_c_string_actual%to_c()
      call c_assert_equal_string(&
            f_c_string_expected%get_c_string(), &
            f_c_string_actual%get_c_string())
   end subroutine assert_equal_string

   subroutine assert_not_equal_int(expected, actual)
      implicit none
      integer(c_int) :: expected, actual
      interface
         subroutine c_assert_not_equal_int(expected, actual) bind(C)
            import :: c_int
            integer(c_int), value :: expected, actual
         end subroutine
      end interface
      call c_assert_not_equal_int(expected, actual)
   end subroutine assert_not_equal_int

   subroutine assert_not_equal_float(expected, actual)
      implicit none
      real(c_float) :: expected, actual
      interface
         subroutine c_assert_not_equal_float(expected, actual) bind(C)
            import :: c_float
            real(c_float), value :: expected, actual
         end subroutine
      end interface
      call c_assert_not_equal_float(expected, actual)
   end subroutine assert_not_equal_float

   subroutine assert_not_equal_double(expected, actual)
      implicit none
      real(c_double) :: expected, actual
      interface
         subroutine c_assert_not_equal_double(expected, actual) bind(C)
            import :: c_double
            real(c_double), value :: expected, actual
         end subroutine
      end interface
      call c_assert_not_equal_double(expected, actual)
   end subroutine assert_not_equal_double

   subroutine assert_not_equal_string(expected, actual)
      implicit none
      character(len = *) :: expected, actual
      type(f_c_string_t) :: f_c_string_expected, f_c_string_actual
      integer :: status
      interface
         subroutine c_assert_not_equal_string(expected, actual) bind(C)
            import :: c_ptr
            type(c_ptr), value, intent(in) :: expected, actual
         end subroutine
      end interface
      status = 0
      f_c_string_expected = f_c_string_t(expected)
      f_c_string_actual = f_c_string_t(actual)
      status = f_c_string_expected%to_c()
      status = f_c_string_actual%to_c()
      call c_assert_not_equal_string(&
            f_c_string_expected%get_c_string(), &
            f_c_string_actual%get_c_string())
   end subroutine assert_not_equal_string

   subroutine assert_true(condition)
      implicit none
      logical, intent(in) :: condition
      integer(c_int) :: i_condition
      interface
         subroutine c_assert_true(condition) bind(C, name = "c_assert_true")
            import :: c_int
            integer(c_int), value :: condition
         end subroutine
      end interface
      i_condition = merge(1_c_int, 0_c_int, condition)
      call c_assert_true(i_condition)
   end subroutine assert_true

   subroutine assert_false(condition)
      implicit none
      logical, intent(in) :: condition
      integer(c_int) :: i_condition
      interface
         subroutine c_assert_false(condition) bind(C, name = "c_assert_false")
            import :: c_int
            integer(c_int), value :: condition
         end subroutine
      end interface
      i_condition = merge(1_c_int, 0_c_int, condition)
      call c_assert_false(i_condition)
   end subroutine assert_false

end module fortest_assert

! f90_test.f90
! 
! To compile this program:
! gfortran -o f90_test -Wall f90_test.f90 -lSDL_bgi -lSDL2
!
! Kindly provided by Angelo Graziosi, angelo.graziosi "at" alice.it

module sdl_bgi
  use, intrinsic :: iso_c_binding, only: C_INT, C_CHAR
  implicit none
  private
  
  interface
    subroutine close_graph() bind(C, name = 'closegraph')
      import
    end subroutine close_graph
  end interface
  
  interface
    function get_ch() bind(C, name = 'getch')
      import
      integer(C_INT) :: get_ch
    end function get_ch
  end interface
  
  interface
    function get_pixel(x,y) bind(C, name = 'getpixel')
      import
      integer(C_INT) :: get_pixel
      integer(C_INT), intent(in), value :: x, y
    end function get_pixel
  end interface
  
  interface
    subroutine init_graph(gd,gm,pd) bind(C, name = 'initgraph')
      import
      integer(C_INT), intent(inout) :: gd, gm
      character(C_CHAR), intent(in) :: pd(*)
    end subroutine init_graph
  end interface
  
  interface
    subroutine put_pixel(x,y,c) bind(C, name = 'putpixel')
      import
      integer(C_INT), intent(in), value :: x, y, c
    end subroutine put_pixel
  end interface
  
  interface
    subroutine out_text_xy(x,y,textstring) bind(C, name = 'outtextxy')
      import
      integer(C_INT), intent(in), value :: x, y
      character(C_CHAR), intent(in) :: textstring(*)
    end subroutine out_text_xy
  end interface
  
  public :: close_graph, get_ch, get_pixel, init_graph, &
&   put_pixel, out_text_xy
  
end module sdl_bgi

program sdl_bgi_test
  use, intrinsic :: iso_c_binding, only: C_NULL_CHAR
  use sdl_bgi, only: close_graph, get_ch, get_pixel, &
&   init_graph, put_pixel, out_text_xy
  implicit none
  
  integer, parameter :: WIDTH = 640, HEIGHT = 480, MAX_COLOR = 15, NPTS = 1000
  integer :: i, x, y, c, k, gd, gm
  real :: r(3)

  gd = 3
  gm = 3
  call init_graph(gd, gm, '')
  
  call random_seed()
  do i = 1, NPTS
    call random_number(r)
    
    x = int(r(1)*WIDTH)
    y = int(r(2)*HEIGHT)
    c = 1+int(r(3)*MAX_COLOR)
    
    call put_pixel(x,y,c)
  end do

  k = get_ch()
  
  call random_seed()
  do i = 1, NPTS
    call random_number(r)
    
    x = int(r(1)*WIDTH)
    y = int(r(2)*HEIGHT)
    c = get_pixel(x,y)
    
    if (c == 1+int(r(3)*MAX_COLOR)) call put_pixel(x,y,0)
  end do
  
  call out_text_xy(0,20,'Press a key to quit'//C_NULL_CHAR)
  k = get_ch()
  call close_graph()
  
end program sdl_bgi_test

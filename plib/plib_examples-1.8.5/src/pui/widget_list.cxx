/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 2001  Steve Baker

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

     For further information visit http://plib.sourceforge.net

     $Id: widget_list.cxx 2077 2006-04-13 16:21:42Z bram $
*/

// Sample program to create a copy of each widget in PUI

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif
#include <math.h>

#ifdef FREEGLUT_IS_PRESENT
#  include <GL/freeglut.h>
#else
#  ifdef __APPLE__
#    include <GLUT/glut.h>
#  else
#    include <GL/glut.h>
#  endif
#endif

#include <plib/pu.h>
#include <plib/puAux.h>

//#define VOODOO 1

/***********************************\
*                                   *
* These are the PUI widget pointers *
*                                   *
\***********************************/

static int          main_window ;

static int button_box_window ;
static puButtonBox *button_box ;

static int frame_window ;
static puFrame *frame ;

static int text_window ;
static puText *text ;

static int button_window ;
static puButton *button ;
static puText *button_text ;

static int one_shot_window ;
static puOneShot *one_shot ;

static int popup_menu_window ;
static puPopupMenu *popup_menu ;

static int menu_bar_window ;
static puMenuBar *menu_bar ;

static int input_window ;
static puInput *input ;

static int slider_window ;
static puSlider *slider ;

static int arrow_button_window ;
static puArrowButton *arrow_button ;

static int dial_window ;
static puDial *dial ;

static int list_box_window ;
static puListBox *list_box ;

static int file_selector_window ;
static puaFileSelector *file_selector ;

static int bislider_window ;
static puaBiSlider *bislider ;

static int trislider_window ;
static puaTriSlider *trislider ;

static int vertical_menu_window ;
static puaVerticalMenu *vertical_menu ;

static int dialog_box_window ;
static puDialogBox *dialog_box ;

static int large_input_window ;
static puaLargeInput *large_input ;

static int combo_box_window ;
static puaComboBox *combo_box ;

static int select_box_window ;
static puaSelectBox *select_box ;

//static fntTexFont *tim ;



/**************************************\
*                                      *
* These three functions capture mouse  *
* and keystrokes (special and mundane) *
* from GLUT and pass them on to PUI.   *
*                                      *
\**************************************/

static void specialfn ( int key, int, int )
{
  puKeyboard ( key + PU_KEY_GLUT_SPECIAL_OFFSET, PU_DOWN ) ;
  glutPostRedisplay () ;
}

static void keyfn ( unsigned char key, int, int )
{
  puKeyboard ( key, PU_DOWN ) ;
  glutPostRedisplay () ;
}

static void motionfn ( int x, int y )
{
  puMouse ( x, y ) ;
  glutPostRedisplay () ;
}

static void mousefn ( int btn, int updown, int x, int y )
{
  puMouse ( btn, updown, x, y ) ;
  glutPostRedisplay () ;
}

/**************************************\
*                                      *
* This function redisplays the PUI     *
*                                      *
\**************************************/

static void displayfn (void)
{
  /* Clear the screen */

  glClearColor ( 0.55f, 0.55f, 0.55f, 1.0f ) ;
  glClear      ( GL_COLOR_BUFFER_BIT ) ;

  /* Make PUI redraw */

  puDisplay () ;

  /* Swap buffers */

  glutSwapBuffers   () ;
}


/***********************************\
*                                   *
* Here are the PUI widget callback  *
* functions.                        *
*                                   *
\***********************************/

static void exit_cb ( puObject * )
{
  fprintf ( stderr, "Exiting PUI demo program.\n" ) ;
  exit ( 1 ) ;
}

/* Menu bar entries: */

static char      *file_submenu    [] = {  "Exit", "Close", "========", "Print", "========", "Save", "New", NULL } ;
static puCallback file_submenu_cb [] = { exit_cb, exit_cb,       NULL, NULL  ,       NULL,  NULL, NULL, NULL } ;

static char      *edit_submenu    [] = { "Do nothing.", NULL } ;
puCallback edit_submenu_cb [] = {     NULL, NULL } ;

static char      *help_submenu    [] = { "About...",  "Help", NULL } ;
puCallback help_submenu_cb [] = {   NULL, NULL, NULL } ;


int main ( int argc, char **argv )
{
  glutInitWindowPosition( 100,   0 ) ;
  glutInitWindowSize    ( 640, 480 ) ;
  glutInit              ( &argc, argv ) ;
  glutInitDisplayMode   ( GLUT_RGB | GLUT_DOUBLE ) ;
  main_window = glutCreateWindow      ( "PUI Widget List"  ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  puInit () ;

#ifdef VOODOO
  puShowCursor () ;
#endif

  //tim = new fntTexFont ;
  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;
  //puFont times_medium ( tim, 12 ) ;
  //puSetDefaultFonts        ( times_medium, times_medium ) ;
  puSetDefaultStyle        ( PUSTYLE_SMALL_SHADED ) ;
  puSetDefaultColourScheme ( 0.8f, 0.8f, 0.8f, 1.0f ) ;


  /* Set up the secondary windows and their widgets */

  button_box_window = glutCreateWindow      ( "Button Box Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  char *button_box_entries [] = { "First Entry", "Second Entry", "Third Entry", NULL } ;
  button_box = new puButtonBox ( 10, 10, 130, 80, button_box_entries, TRUE ) ;
  button_box->setLabel ( "Label" ) ;


  frame_window = glutCreateWindow      ( "Frame Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  frame = new puFrame ( 10, 10, 130, 80 ) ;
  frame->setLabel ( "Label" ) ;
  frame->setLegend ( "Legend" ) ;


  text_window = glutCreateWindow      ( "Text Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  text = new puText ( 10, 10 ) ;
  text->setLabel ( "Label" ) ;


  button_window = glutCreateWindow      ( "Button Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 220,  160 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  button_text = new puText ( 5, 6 ) ;
  button_text->setLabel ( "XCHECK:" ) ;

  button = new puButton ( 90, 10, 90+16, 10+16, PUBUTTON_XCHECK ) ;
  button->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  button->setLabel ( "Label" ) ;
  button->setValue ( 1 ) ;

  button_text = new puText ( 5, 36 ) ;
  button_text->setLabel ( "VCHECK:" ) ;

  button = new puButton ( 90, 40, 90+16, 40+16, PUBUTTON_VCHECK ) ;
  button->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  button->setLabel ( "Label" ) ;
  button->setValue ( 1 ) ;

  button_text = new puText ( 5, 66 ) ;
  button_text->setLabel ( "CIRCLE:" ) ;

  button = new puButton ( 90, 70, 90+16, 70+16, PUBUTTON_CIRCLE ) ;
  button->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  button->setLabel ( "Label" ) ;
  button->setValue ( 1 ) ;

  button_text = new puText ( 5, 96 ) ;
  button_text->setLabel ( "RADIO:" ) ;

  button = new puButton ( 90, 100, 90+16, 100+16, PUBUTTON_RADIO ) ;
  button->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  button->setLabel ( "Label" ) ;
  button->setValue ( 1 ) ;

  button_text = new puText ( 5, 128 ) ;
  button_text->setLabel ( "NORMAL:" ) ;

  button = new puButton ( 90, 130, 170, 150 ) ;
  button->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
  button->setLabel ( "Label" ) ;
  button->setLegend ( "Legend" ) ;
  button->setValue ( 1 ) ;


  one_shot_window = glutCreateWindow      ( "One Shot Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  one_shot = new puOneShot ( 10, 10, 90, 30 ) ;
  one_shot->setLabel ( "Label" ) ;
  one_shot->setLegend ( "Legend" ) ;


  popup_menu_window = glutCreateWindow      ( "Popup Menu Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 80,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  popup_menu = new puPopupMenu ( 10, 10 ) ;
  popup_menu->add_item ( "Cut", NULL ) ;
  popup_menu->add_item ( "Copy", NULL ) ;
  popup_menu->add_item ( "Paste", NULL ) ;
  popup_menu->add_item ( "Delete", NULL ) ;
  popup_menu->close () ;
  popup_menu->reveal () ;


  menu_bar_window = glutCreateWindow      ( "Menu Bar Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  200 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  menu_bar = new puMenuBar () ;
  {
    menu_bar -> add_submenu ( "File", file_submenu, file_submenu_cb ) ;
    menu_bar -> add_submenu ( "Edit", edit_submenu, edit_submenu_cb ) ;
    menu_bar -> add_submenu ( "Help", help_submenu, help_submenu_cb ) ;
  }
  menu_bar -> close () ; 


  input_window = glutCreateWindow      ( "Input Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  input = new puInput ( 10, 10, 90, 30 ) ;
  input->setLabel ( "Label" ) ;


  slider_window = glutCreateWindow      ( "Slider Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  slider = new puSlider ( 10, 10, 120, FALSE ) ;
  slider->setLabel ( "Label" ) ;
  slider->setLegend ( "Legend" ) ;


  arrow_button_window = glutCreateWindow      ( "Arrow Button Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  arrow_button = new puArrowButton ( 10, 10, 50, 50, PUARROW_RIGHT ) ;
  arrow_button->setLabel ( "Label" ) ;


  dial_window = glutCreateWindow      ( "Dial Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  dial = new puDial ( 10, 10, 60 ) ;
  dial->setLabel ( "Label" ) ;
  dial->setLegend ( "Legend" ) ;


  list_box_window = glutCreateWindow      ( "List Box Window" ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180, 100  ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  char *list_box_entries [] = { "First Entry", "Second Entry", "Third Entry", "Very long Entry that will get truncated", NULL } ;
  list_box = new puListBox ( 10, 10, 130, 80, list_box_entries ) ;
  list_box->setLabel ( "Label" ) ;


  file_selector_window = glutCreateWindow      ( "File Selector Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 300,  220 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  file_selector = new puaFileSelector ( 10, 10, 280, 200, "." ) ;


  bislider_window = glutCreateWindow      ( "BiSlider Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  bislider = new puaBiSlider ( 10, 10, 120, FALSE ) ;
  bislider->setLabel ( "Label" ) ;
  bislider->setLegend ( "Legend" ) ;
  bislider->setMaxValue ( 20 ) ;
  bislider->setCurrentMin ( 4 ) ;
  bislider->setCurrentMax ( 15 ) ;


  trislider_window = glutCreateWindow      ( "TriSlider Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 100,  200 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  trislider = new puaTriSlider ( 20, 10, 180, TRUE ) ;
  trislider->setLabel ( "Label" ) ;
  trislider->setLegend ( "Legend" ) ;
  trislider->setMaxValue ( 20 ) ;
  trislider->setCurrentMin ( 4 ) ;
  trislider->setCurrentMax ( 15 ) ;
  trislider->setValue ( 12 ) ;


  vertical_menu_window = glutCreateWindow      ( "Vertical Menu Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  200 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  vertical_menu = new puaVerticalMenu () ;
  {
    vertical_menu -> add_submenu ( "File", file_submenu, file_submenu_cb ) ;
    vertical_menu -> add_submenu ( "Edit", edit_submenu, edit_submenu_cb ) ;
    vertical_menu -> add_submenu ( "Help", help_submenu, help_submenu_cb ) ;
  }
  vertical_menu -> close () ; 


  dialog_box_window = glutCreateWindow      ( "Dialog Box Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 180,  100 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  dialog_box = new puDialogBox ( 10, 10 ) ;
  new puOneShot ( 20, 20, "OK" ) ;
  new puOneShot ( 100, 20, "Cancel" ) ;
  puText *dialog_text = new puText ( 20, 60 ) ;
  dialog_text->setLabel ( "Dialog Box Text\nand Widgets" ) ;
  dialog_box->close () ;
  dialog_box->reveal () ;


  large_input_window = glutCreateWindow      ( "Large Input Window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 300,  200 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  large_input = new puaLargeInput ( 10, 10, 240, 180, 2, 20 ) ;
  large_input->setLabel ( "Label" ) ;
  large_input->setText ( "This is text in the Large Input widget.\n"
                         "This is a second line of text" ) ;


  combo_box_window = glutCreateWindow      ( "Combo Box window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 300,  200 ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  char *combo_box_entries [] = { "First Entry", "Second Entry", "Third Entry", "Fourth Entry", "Sixth Entry", "Seventh Entry", NULL } ;
  combo_box = new puaComboBox ( 10, 158, 250, 190,
                               combo_box_entries ) ;
  /* Don't do that in your own application ! */
  combo_box -> __getPopupMenu() -> reveal() ;
  combo_box->setLabel ( "Label" ) ;


  select_box_window = glutCreateWindow      ( "Select Box window"  ) ;
  glutPositionWindow    ( 200, 100 ) ;
  glutReshapeWindow     ( 300, 60   ) ;
  glutDisplayFunc       ( displayfn ) ;
  glutKeyboardFunc      ( keyfn     ) ;
  glutSpecialFunc       ( specialfn ) ;
  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
#ifdef VOODOO
  glutPassiveMotionFunc ( motionfn  ) ;
#endif

  //tim -> load ( "../fnt/data/old/times_bold.txf" ) ;

  glEnable ( GL_BLEND ) ;
  glAlphaFunc ( GL_GEQUAL, 0.1f ) ;
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

  select_box = new puaSelectBox ( 10, 14, 250, 46,
                                 combo_box_entries ) ;
  select_box->setLabel ( "Label" ) ;

  glutMainLoop () ;
  return 0 ;
}



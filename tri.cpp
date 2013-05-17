#include <GL/glut.h>
#include <stdlib.h>

int width = 300, height = 600;

/* 初期値 */
float y = .0;
float Vy = 200.;
float g = 9.8;
float t = .0;
float dt = .1;
float x = -1.;
float Vx = .03;

float yi[1000];  /* 軌跡データ格納用 */
float xi[1000];
int i=0, j=0;

void FreeFall(void){
  /* axis */
  glColor3f(.2, .3, .5);
  glBegin( GL_LINES );
  glVertex2d(-1, 0);
  glVertex2d(1, 0);
  glEnd();

  /* draw points */
  glColor3f( .9, .2, .2 );
  glPointSize(4.);
  glBegin( GL_POINTS );
  glVertex2f( x, y);
  glEnd();

  /* draw orbits */
  glColor3f( .5, .2, .2 );
  glBegin( GL_LINE_STRIP );
  for(j=0 ; j<i ; j++)
    glVertex2f( xi[j], yi[j]);
  glEnd();

  /* draw points par 10 */
  glColor3f( 1., 1., 1. );
  glPointSize(4.);
  glBegin( GL_POINTS );
  for(j=0 ; j<i ; j+=50)
    glVertex2f( xi[j], yi[j]);
  glEnd();
}

void display(void){
  glClearColor( .0, .0, .0, .0);
  glClear( GL_COLOR_BUFFER_BIT );
  FreeFall();
  glutSwapBuffers();
}

void idle(void){
  float a;

  y += Vy*dt;
  a = -g;  /* 加速度一定, 下向き */
  t += dt;
  Vy += a*dt;
  x += Vx*dt;
  
  /* 軌跡データ格納 */
  yi[i] = y;
  xi[i] = x;
  i++;
  
  /* 画面右端で停止 */
  if(x>=1.)
    glutIdleFunc( NULL );

  glutPostRedisplay();
}

int main(int argc, char** argv){
  /* init gult */
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowPosition( 0, 0 );
  glutInitWindowSize( width, height );
  glutCreateWindow( "FreeFall" );

  /* call-back */
  glutDisplayFunc( display );
  glutIdleFunc( idle );

  /* 2D-setup */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( -1., 1., -10000., 5000. );

  glutMainLoop();
  return 0;
}

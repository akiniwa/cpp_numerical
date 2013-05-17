#include <GL/glut.h>
#include <iostream>

// this is a simple example of a 3D interactive program

float rx,ry, rw, rh;

float bx, by;
float dx, dy;

float winWid, winHeight;
void redraw();

void timer(int timeCG) {
		redraw();
		glutTimerFunc(10 , timer , 0);
}

void redraw()
{
  int i;
  float px, py;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // here is the floor
  glColor3f(0.6,0.6,0.0);
  glPushMatrix();
  glTranslatef(0.0,-10.0,-10.0);
  glRotatef(90.0,1.0,0.0,0.0);
  for(i=0;i<10;++i)
    {
      glRectf(-10.0, float(i)*2.0, 10.0, float(i)*2.0 + 1.0);
    }

  glColor3f(1.0,0.0,0.0);

  px = rx/20.0-10.0;
  py = 10.0-ry/20.0;
  glTranslatef(0.0,0.0,-0.003);
  glRectf(px - 2.0,py -2.0, px + 2.0, py + 2.0);

  glPopMatrix();

  // the ball
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glPushMatrix();
  glTranslatef(bx,by,-0.150);
  glRotatef(90.0,0.0,1.0,0.0);
  glutSolidSphere( 2.0,10,10);
  glPopMatrix();
  glDisable(GL_LIGHTING);

  // the shadow
  glColor3f(0.4,0.4,0.0);
  glPushMatrix();
  glTranslatef(bx,-10.0,-0.0150);
  glScalef(1.0,0.001,1.0);
  glutSolidSphere( 2.0,10,10);
  glPopMatrix();

  glutSwapBuffers();

  bx += dx;
  by += dy;

  if (by > 10.0) dy = -dy;
  if (bx > 10.0) dx = -dx;

  if (by < -8.0) dy = -dy; // floor + radius
  if (bx < -10.0) dx = -dx;

  dy = dy - 0.003; // this is gravity
}

void mousebutton(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
      rx = x; ry = winHeight - y;
    }
}

void motion(int x, int y) 
// called when a mouse is in motion with a button down
{

  rx = x; ry = winHeight - y;
}

void keyboard(unsigned char key, int x, int y) // x and y give the mouse pos
{
  //cerr << "Key " << key << " int " << int(key) << "\n";
}

void reshape(int w, int h)
{

  winWid = w, winHeight = h;
  glViewport(0.0,0.0,winWid,winHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-10.0,10.0,-10.0,10.0, 20.0, 2000.0);
  glTranslatef(0.0,0.0,-25.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char *argv[])
{
  //cerr << "hello world\n";

  rx = 100; ry = 200; rw = 30; rh = 15;

  bx = 0; by = 0;
  dx = 0.06; dy = 0.08;
  winWid = 500; winHeight = 500;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Mouse example");
  glutPositionWindow(200,100);
  glutReshapeWindow(winWid,winHeight);

  glClearColor(0.5,0.5,0.5,1.0);

  float light_position[] = {-10.0,20.0,20.0,1.0};
  glLightfv(GL_LIGHT0,GL_POSITION, light_position);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-10.0,10.0,-10.0,10.0, 20.0, 2000.0);
  glTranslatef(0.0,0.0,-25.0);

  glMatrixMode(GL_MODELVIEW);;
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(redraw);
glutTimerFunc(10 , timer , 0);
  glutReshapeFunc(reshape);
//  glutIdleFunc(redraw);
  glutMouseFunc( mousebutton);
  glutKeyboardFunc(keyboard);
  glutMotionFunc( motion);

  glutMainLoop();

  return 0;
}

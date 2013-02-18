#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define MAXPOINTS 100
GLint point[MAXPOINTS][2];
int pointnum = 0;
int rubberband = 0;

void display() {
  int i;
  // window$B$rEI$j$D$V$9(B
  glClear(GL_COLOR_BUFFER_BIT);
  if (pointnum > 1) {
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (i=0;i<pointnum;++i) {
	  glVertex2iv(point[i]);
	}
	glEnd();
  }
  /*
  // $BIA<L$9$k$b$N$N?'$r;XDj$9$k(B
  glBegin(GL_POLYGON);
  //  glBegin(GL_LINES);
  glColor3d(1.0, 1.0, 0.0);
  glVertex2d(-0.9, -0.9);
  glColor3d(1.0, 0.0, 0.0);
  glVertex2d(0.9, -0.9);
  glColor3d(0.0, 1.0, 0.0);
  glVertex2d(0.9, 0.9);
  glColor3d(0.0, 0.0, 1.0);
  glVertex2d(-0.9, 0.9);
  glEnd();
  */
  // $B<B9T$5$l$F$$$J$$L?Na$r<B9T$9$k(B
  glFlush();
}

void resize(int w, int h) {
  // window$BA4BN$r(Bviewport$B$K$9$k(B
  glViewport(0, 0, w, h);
  // $BJQ499TNs$N=i4|2=(B
  glLoadIdentity();
  // $B%9%/%j!<%s$NI=<(NN0h$r(Bviewport$B$NBg$-$5$KHfNc$5$;$k(B
  //  glOrtho(-w/200.0, w/200.0, -h/200.0, h/200.0, -1.0, 1.0);
  // $B%9%/%j!<%s>e$N:BI87O$r%^%&%9$N:BI87O$K0lCW$5$;$k(B
  glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y) {
  static int x0, y0;
  switch (button) {
	case GLUT_LEFT_BUTTON:
	  point[pointnum][0] = x;
	  point[pointnum][1] = y;
	  if (state==GLUT_UP) {
	  glColor3d(0.0, 0.0, 0.0);
	  glBegin(GL_LINES);
	  glVertex2iv(point[pointnum - 1]);
	  glVertex2iv(point[pointnum]);
	  glEnd();
	  glFlush();
	  rubberband = 0;
	  printf("left");
	  } else {
	  }
	  if (pointnum < MAXPOINTS - 1) {
		++pointnum;
	  }
	  break;
	case GLUT_MIDDLE_BUTTON:
	  printf("middle");
	  break;
	case GLUT_RIGHT_BUTTON:
	  printf("right");
	  break;
	default:
	  break;
  }
/*  
  printf("button is");

  switch (state) {
	case GLUT_UP:
	  printf("up");
	  break;
	case GLUT_DOWN:
	  printf("down");
	  break;
	default:
	  break;
  }
  printf(" at (%d, %d)\n", x, y);
*/
}

void motion(int x, int y) {
  static GLint savepoint[2];
  
  // $BO@M}1i;;5!G=(B ON
  glEnable(GL_COLOR_LOGIC_OP);
  glLogicOp(GL_INVERT);

  glBegin(GL_LINES);
  if (rubberband) {
	glVertex2iv(point[pointnum - 1]);
	glVertex2iv(savepoint);
  }
  glVertex2iv(point[pointnum - 1]);
  glVertex2i(x, y);
  glEnd();
  glFlush();

  // $BO@M}1i;;5!G=(B OFF
  glLogicOp(GL_COPY);
  glDisable(GL_COLOR_LOGIC_OP);

  // $B:#IA$$$?%i%P!<%P%s%I$NC<E@$rJ]B8(B
  savepoint[0] = x;
  savepoint[1] = y;

  // $B:#IA$$$?%i%P!<%P%s%I$O<!$N%?%$%_%s%0$G>C$9(B
  rubberband = 1;
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
	case 'q':
	case 'Q':
	case '\033': //ESC
	  printf("\nexit\n");
	  exit(0);
	default:
	  break;
  }
}

void init()
{
  // window$B$rEI$j$D$V$9?'$N;XDj(B
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char * argv[]) {
  // $B?7$?$K3+$/(Bwindow$B$NI}$H9b$5$r;XDj$9$k(B($B%G%U%)%k%H(B300x300)
  glutInitWindowPosition(100, 100);
  // window$B$N0LCV$r;XDj(B
  glutInitWindowSize(320, 240);
  // GLUT$B$*$h$S(BOpenGL$B4D6-$r=i4|2=(B
  glutInit(&argc, argv);
  // $B%G%#%9%W%l%$$NI=<(%b!<%I$r@_Dj(B
  glutInitDisplayMode(GLUT_RGBA);
  // window$B$r3+$/(B
  glutCreateWindow(argv[0]);
  // $B4X?t$X$N%]%$%s%?$r;XDj(B
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  // $BL58B%k!<%W(B
  glutMainLoop();

}

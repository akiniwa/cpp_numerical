#include <stdlib.h>
#include <GL/glut.h>

GLdouble vertex[][3] = {
  { 0.0, 0.0, 0.0 }, /*  A */
  { 1.0, 0.0, 0.0 }, /*  B */
  { 1.0, 1.0, 0.0 }, /*  C */
  { 0.0, 1.0, 0.0 }, /*  D */
  { 0.0, 0.0, 1.0 }, /*  E */
  { 1.0, 0.0, 1.0 }, /*  F */
  { 1.0, 1.0, 1.0 }, /*  G */
  { 0.0, 1.0, 1.0 }  /*  H */
};

int face[][4] = {
  { 0, 1, 2, 3 }, /*  A-B-C-D $B$r7k$VLL(B */
  { 1, 5, 6, 2 }, /*  B-F-G-C $B$r7k$VLL(B */
  { 5, 4, 7, 6 }, /*  F-E-H-G $B$r7k$VLL(B */
  { 4, 0, 3, 7 }, /*  E-A-D-H $B$r7k$VLL(B */
  { 4, 5, 1, 0 }, /*  E-F-B-A $B$r7k$VLL(B */
  { 3, 2, 6, 7 }  /*  D-C-G-H $B$r7k$VLL(B */
};

GLdouble color[][3] = {
  { 1.0, 0.0, 0.0 }, /*  $B@V(B */
  { 0.0, 1.0, 0.0 }, /*  $BNP(B */
  { 0.0, 0.0, 1.0 }, /*  $B@D(B */
  { 1.0, 1.0, 0.0 }, /*  $B2+(B */
  { 1.0, 0.0, 1.0 }, /*  $B%^%<%s%?(B */
  { 0.0, 1.0, 1.0 }  /*  $B%7%"%s(B $B!!(B*/
};

int edge[][2] = {
  { 0, 1 }, /*  $B%"(B (A-B) */
  { 1, 2 }, /*  $B%$(B (B-C) */
  { 2, 3 }, /*  $B%&(B (C-D) */
  { 3, 0 }, /*  $B%((B (D-A) */
  { 4, 5 }, /*  $B%*(B (E-F) */
  { 5, 6 }, /*  $B%+(B (F-G) */
  { 6, 7 }, /*  $B%-(B (G-H) */
  { 7, 4 }, /*  $B%/(B (H-E) */
  { 0, 4 }, /*  $B%1(B (A-E) */
  { 1, 5 }, /*  $B%3(B (B-F) */
  { 2, 6 }, /*  $B%5(B (C-G) */
  { 3, 7 }  /*  $B%7(B (D-H) */
};

GLdouble normal[][3] = {
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  {-1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };
GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
void cube() {
  int i;
  int j;

  glBegin(GL_QUADS);
  for (j=0;j<6;++j) {
	glNormal3dv(normal[j]);
	for (i=0;i<4;++i) {
	  glVertex3dv(vertex[face[j][i]]);
	}
  }
  glEnd();
}

void idle(void) {
  glutPostRedisplay();
}

void display() {
  int i;
  int j;
  static int r = 0;
  // window$B$rEI$j$D$V$9(B
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
  glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

  glPushMatrix();

  glRotated((double)r, 0.0, 1.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

  cube();
  glPushMatrix();
  glTranslated(1.0, 1.0, 1.0);
  glRotated((double)(2*r), 0.0, 1.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
  cube();
  glPopMatrix();
 

  glutSwapBuffers();
  if (++r >= 360) r = 0;
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void mouse(int button, int state, int x, int y) {
  switch (button) {
	case GLUT_LEFT_BUTTON:
	  if (state==GLUT_DOWN) {
		glutIdleFunc(idle);
	  } else {
		glutIdleFunc(0);
	  }
	  break;
	case GLUT_RIGHT_BUTTON:
	  if (state==GLUT_DOWN) {
		glutPostRedisplay();
	  }
	  break;
	default:
	  break;
  }
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
	case 'q':
  	case 'Q':
	case '\033':
	  exit(0);
	default:
	  break;
  }
}

void init()
{
  // window$B$rEI$j$D$V$9?'$N;XDj(B
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT1, GL_SPECULAR, green);
}

int main(int argc, char * argv[]) {
  // GLUT$B$*$h$S(BOpenGL$B4D6-$r=i4|2=(B
  glutInit(&argc, argv);
  // $B%G%#%9%W%l%$$NI=<(%b!<%I$r@_Dj(B
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  // window$B$r3+$/(B
  glutCreateWindow(argv[0]);
  // $B4X?t$X$N%]%$%s%?$r;XDj(B
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();
  // $BL58B%k!<%W(B
  glutMainLoop();
}

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
  { 0, 1, 2, 3 }, /*  A-B-C-D を結ぶ面 */
  { 1, 5, 6, 2 }, /*  B-F-G-C を結ぶ面 */
  { 5, 4, 7, 6 }, /*  F-E-H-G を結ぶ面 */
  { 4, 0, 3, 7 }, /*  E-A-D-H を結ぶ面 */
  { 4, 5, 1, 0 }, /*  E-F-B-A を結ぶ面 */
  { 3, 2, 6, 7 }  /*  D-C-G-H を結ぶ面 */
};

GLdouble color[][3] = {
  { 1.0, 0.0, 0.0 }, /*  赤 */
  { 0.0, 1.0, 0.0 }, /*  緑 */
  { 0.0, 0.0, 1.0 }, /*  青 */
  { 1.0, 1.0, 0.0 }, /*  黄 */
  { 1.0, 0.0, 1.0 }, /*  マゼンタ */
  { 0.0, 1.0, 1.0 }  /*  シアン 　*/
};

int edge[][2] = {
  { 0, 1 }, /*  ア (A-B) */
  { 1, 2 }, /*  イ (B-C) */
  { 2, 3 }, /*  ウ (C-D) */
  { 3, 0 }, /*  エ (D-A) */
  { 4, 5 }, /*  オ (E-F) */
  { 5, 6 }, /*  カ (F-G) */
  { 6, 7 }, /*  キ (G-H) */
  { 7, 4 }, /*  ク (H-E) */
  { 0, 4 }, /*  ケ (A-E) */
  { 1, 5 }, /*  コ (B-F) */
  { 2, 6 }, /*  サ (C-G) */
  { 3, 7 }  /*  シ (D-H) */
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
  // windowを塗りつぶす
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
  // windowを塗りつぶす色の指定
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
  // GLUTおよびOpenGL環境を初期化
  glutInit(&argc, argv);
  // ディスプレイの表示モードを設定
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  // windowを開く
  glutCreateWindow(argv[0]);
  // 関数へのポインタを指定
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();
  // 無限ループ
  glutMainLoop();
}

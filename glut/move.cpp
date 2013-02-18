/*
 * =====================================================================================
 *
 *       Filename:  move.cpp
 *
 *    Description:  sample for openGL
 *
 *        Version:  1.0
 *        Created:  2013/01/12 13時55分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

void scene(void)
{
  /*  物体の色 */
  static GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
  static GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };
  static GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
  static GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };
  static GLfloat ground[][4] = {
	{ 0.6, 0.6, 0.6, 1.0 },
	{ 0.3, 0.3, 0.3, 1.0 }
  };

  int i, j;

  /*  赤い箱 */
  glPushMatrix();
  glTranslated(0.0, 0.0, -3.0);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glutSolidCube(1.0);
  glPopMatrix();

  /*  緑の箱 */
  glPushMatrix();
  glTranslated(0.0, 0.0, 3.0);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
  glutSolidCube(1.0);
  glPopMatrix(); 
  /*  青い箱 */
  glPushMatrix();
  glTranslated(-3.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
  glutSolidCube(1.0);
  glPopMatrix();

  /*  黄色い箱 */
  glPushMatrix();
  glTranslated(3.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
  glutSolidCube(1.0);
  glPopMatrix();

  /*  地面 */
  glBegin(GL_QUADS);
  glNormal3d(0.0, 1.0, 0.0);
  for (j = -5; j < 5; ++j) {
	for (i = -5; i < 5; ++i) {
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
	  glVertex3d((GLdouble)i, -0.5, (GLdouble)j);
	  glVertex3d((GLdouble)i, -0.5, (GLdouble)(j + 1));
	  glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)(j + 1));
	  glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)j);
	}
  }
  glEnd();
}

void display(void)
{
  static GLfloat lightpos[] = { 3.0, 4.0, 5.0, 1.0 }; /*  光源の位置 */

  static double ex = 0.0, ez = 0.0; /*  視点の位置 */
  static double r = 0.0;            /*  視点の向き */

  /*  画面クリア */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*  モデルビュー変換行列の初期化 */
  glLoadIdentity();

  /*  視点の移動 */
  glRotated(r, 0.0, 1.0, 0.0);
  glTranslated(ex, 0.0, ez);

  /*  光源の位置を設定 */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  /*  シーンの描画 */
  scene();

  glFlush();
}

void resize(int w, int h)
{
  /*  ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);

  /*  透視変換行列の指定 */
  glMatrixMode(GL_PROJECTION);

  /*  透視変換行列の初期化 */
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  /*  モデルビュー変換行列の指定 */
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
  /*  ESC か q をタイプしたら終了 */
  if (key == '\033' || key == 'q') {
	exit(0);
  }
}

void mouse(int button, int state, int x, int y) {
  /*  画面クリア */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  static int rotation_thete = 0.0;
  /*  モデルビュー変換行列の初期化 */
  glLoadIdentity();
  /*  視点の移動 */
  glRotated(rotation_thete, 0.0, 1.0, 0.0);
  glTranslated(0.0, 0.0, 0.0);
  printf("button:%d, state:%d, x:%d, y:%d \n", button, state, x, y);

  static GLfloat lipos[] = {3.0, 4.0, 5.0, 1.0};
  /*  光源の位置を設定 */
  glLightfv(GL_LIGHT0, GL_POSITION, lipos );

  rotation_thete++;
  /*  シーンの描画 */
  scene();
  glFlush();
}

void init(void)
{
  /*  初期設定 */
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  init();
  glutMainLoop();
  return 0;
}

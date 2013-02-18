#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define MAXPOINTS 100
GLint point[MAXPOINTS][2];
int pointnum = 0;
int rubberband = 0;

void display() {
  int i;
  // windowを塗りつぶす
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
  // 描写するものの色を指定する
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
  // 実行されていない命令を実行する
  glFlush();
}

void resize(int w, int h) {
  // window全体をviewportにする
  glViewport(0, 0, w, h);
  // 変換行列の初期化
  glLoadIdentity();
  // スクリーンの表示領域をviewportの大きさに比例させる
  //  glOrtho(-w/200.0, w/200.0, -h/200.0, h/200.0, -1.0, 1.0);
  // スクリーン上の座標系をマウスの座標系に一致させる
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
  
  // 論理演算機能 ON
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

  // 論理演算機能 OFF
  glLogicOp(GL_COPY);
  glDisable(GL_COLOR_LOGIC_OP);

  // 今描いたラバーバンドの端点を保存
  savepoint[0] = x;
  savepoint[1] = y;

  // 今描いたラバーバンドは次のタイミングで消す
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
  // windowを塗りつぶす色の指定
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char * argv[]) {
  // 新たに開くwindowの幅と高さを指定する(デフォルト300x300)
  glutInitWindowPosition(100, 100);
  // windowの位置を指定
  glutInitWindowSize(320, 240);
  // GLUTおよびOpenGL環境を初期化
  glutInit(&argc, argv);
  // ディスプレイの表示モードを設定
  glutInitDisplayMode(GLUT_RGBA);
  // windowを開く
  glutCreateWindow(argv[0]);
  // 関数へのポインタを指定
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  // 無限ループ
  glutMainLoop();

}

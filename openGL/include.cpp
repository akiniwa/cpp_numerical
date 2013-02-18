#include <GLUT/glut.h>

void display(void) {

		glClearColor(255, 255, 255, 0);
		glColor3d(0, 0, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, 128);
		glVertex2i(128, 128);
		glVertex2i(128, 0);
		glEnd();

		glRecti(200, 200, 250, 250);

		glutSwapBuffers();

}

int main(){

		return 0;
}

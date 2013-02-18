#include <GLUT/glut.h>

int width = 1000, height = 1000;

float yi[3000];  /* 軌跡データ格納用 */
float xi[3000];
int i=0, j=0;
FILE *fp3;

void FreeFall(void){
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable(GL_DEPTH_TEST);
  
		glColor3f(1.0,0.1, 0.1);
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidSphere( 0.3,30,30);
		glPopMatrix();

		/* axis */
		glColor3f(.2, .3, .5);
		glBegin( GL_LINES );
		glVertex3d(-1, 0, 0);
		glVertex3d(1, 0, 0);
		glEnd();

		glColor3f( .5, .2, .2 );
		glBegin( GL_LINE_STRIP );
		for(j=0 ; j<i ; j++) {
			glVertex3f( xi[j], yi[j], 0);
		}
		glEnd();

		glColor3f( 1., 1., 1. );
		glPointSize(4.);
		glBegin( GL_POINTS );
		for (j=0;j<i;j+=40) {
			glVertex3f( xi[j], yi[j], 0);
		}
			glVertex3f( xi[i-1], yi[i-1], 0);
		glEnd();

		glBegin( GL_LINE_STRIP );
		for(j=0 ; j<i ; j++) {
			glVertex3f( xi[j]*1.6, yi[j], 0.0f);
		}
		glEnd();

		glColor3f(0.3,0.7, 0.8);
		glPushMatrix();
		glTranslatef( xi[i-1]*1.6, yi[i-1],0.0f);
		glutSolidSphere(0.1,30,30);
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
}

void display(void){
		glClearColor( .0, .0, .0, .0);
		glClear( GL_COLOR_BUFFER_BIT );
		glRotated(0.3, 0.8, 1.0, 0.6);
		FreeFall();
		glutSwapBuffers();

}

void timer(int value) {
		glutTimerFunc(20 , timer , 0);
		i++;
		glutPostRedisplay();
}

void openGL(){
		if ((fp3 = fopen("file", "r")) == NULL) {
			printf("cannot open file..");
			exit(0);
		}

		char data[50];
		int l = 0;
		while (fgets(data, 50, fp3)) {
			sscanf(data, "%f%f",&xi[l], &yi[l]);
			l++;
		}

		/* init gult */
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowPosition( 0, 0 );
		glutInitWindowSize( width, height );
		glutCreateWindow( "FreeFall" );

		/* call-back */
		glutDisplayFunc( display );
		glutTimerFunc(100 , timer , 0);

		/* 2D-setup */
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		/*  
		gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		gluPerspective(30.0, (double)width / (double)height, 1.0, 100.0);
		glTranslated(0.0, 0.0, -0.6);
*/
gluPerspective(30.0, (double)width / (double)height, 1.0, 100.0);
//		glOrtho(-2.0, 2.0, -2.0, 2.0, -8.0, 8.0);
		glTranslated(0.0, 0.0, -5.0);

//		gluOrtho2D( -1.7, 1.7, -1.7, 1.7 );

		glutMainLoop();
}

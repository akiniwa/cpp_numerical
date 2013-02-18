#include <GLUT/glut.h>

int width = 500, height = 500;

float yi[3000];  /* 軌跡データ格納用 */
float xi[3000];
int i=0, j=0;
FILE *fp3;

void FreeFall(void){
		/* axis */
		glColor3f(.2, .3, .5);
		glBegin( GL_LINES );
		glVertex2d(-1, 0);
		glVertex2d(1, 0);
		glEnd();

		/* draw orbits */
		glColor3f( .5, .2, .2 );
		glBegin( GL_LINE_STRIP );
		for(j=0 ; j<i ; j++)
			glVertex2f( xi[j], yi[j]);
		glEnd();

		glColor3f( 1., 1., 1. );
		glPointSize(4.);
		glBegin( GL_POINTS );

		for (j=0;j<i;j+=40) {
			glVertex2f( xi[j], yi[j]);
		}
			glVertex2f( xi[i-1], yi[i-1]);

		glEnd();
}

void display(void){
		glClearColor( .0, .0, .0, .0);
		glClear( GL_COLOR_BUFFER_BIT );
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
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
		glutInitWindowPosition( 0, 0 );
		glutInitWindowSize( width, height );
		glutCreateWindow( "FreeFall" );

		/* call-back */
		glutDisplayFunc( display );
		glutTimerFunc(100 , timer , 0);

		/* 2D-setup */
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( -1.0, 1.0, -1.0, 1.0 );

		glutMainLoop();
}

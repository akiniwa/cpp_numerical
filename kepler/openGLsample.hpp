#ifdef __APPLE__
#include <GLUT/glut.h>
#elif __linux
#include <GL/glut.h>
#endif

int width = 1000, height = 1000;

float yi[3000];  /* 軌跡データ格納用 */
float xi[3000];
int i=0, j=0;
FILE *fp3;

GLfloat light0pos[] = { 5.0, 0.0, 0.0, 1.0 };
GLfloat light1pos[] = { 2.0, 6.0, 0.0, 1.0 };

struct MaterialStruct {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat shininess;
};

MaterialStruct ms_jade = {
		{0.135,     0.2225,   0.1575,   1.0},
		{0.54,      0.89,     0.63,     1.0},
		{0.316228,  0.316228, 0.316228, 1.0},
		12.8};


MaterialStruct ms_ruby  = {
		{0.1745,   0.01175,  0.01175,   1.0},
		{0.61424,  0.04136,  0.04136,   1.0},
		{0.727811, 0.626959, 0.626959,  1.0},
		76.8};

GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 }; //赤色
GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };//緑色
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };//青色
GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };//黄色
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };//白色
GLfloat shininess = 30.0;//光沢の強さ

void FreeFall(void){
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable(GL_DEPTH_TEST);

		glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
		glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidSphere( 0.3,30,30);
		glPopMatrix();

		/* axis */
		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
		glBegin( GL_LINES );
		glVertex3d(-1, 0, 0);
		glVertex3d(1, 0, 0);
		glEnd();

		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
		glBegin( GL_LINE_STRIP );
		for(j=0 ; j<i ; j++) {
			glVertex3f( xi[j], yi[j], 0);
		}
		glEnd();

		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
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

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
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
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, green);


		glutMainLoop();
}

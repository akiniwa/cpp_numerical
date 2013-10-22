#ifdef __APPLE__
#include <GLUT/glut.h>
#elif __linux
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

int width = 1000, height = 1000;

float yi[1000002];  /* 軌跡データ格納用 */
float xi[1000002];
float zi[1000002];
int count = 0;
int i=0, j=0;
FILE *fp3;
std::ofstream fout;

GLfloat light0pos[] = { 1.0, 1.0, 3.0, 1.0 };
GLfloat light1pos[] = { 2.0, 6.0, 0.0, 1.0 };
GLfloat light2pos[] = { 4.0, 4.0, 3.0, 1.0 };
GLfloat light3pos[] = { 5.0, 3.0, 3.0, 1.0 };

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

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
        glColor3d(0.2, 0.7, 0.2);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidSphere(0.3,30,30);
		glPopMatrix();

		/* X-axis */
		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
		glBegin( GL_LINES );
        glColor3d(1, 0, 0);
		glVertex3d(-10, 0, 0);
		glVertex3d(10, 0, 0);
		glEnd();

		/* Y-axis */
		glBegin( GL_LINES );
        glColor3d(0, 1, 0);
		glVertex3d(0, -10, 0);
		glVertex3d(0, 10, 0);
		glEnd();

		/* Z-axis */
		glBegin( GL_LINES );
        glColor3d(0, 0, 1);
		glVertex3d(0, 0, -10);
		glVertex3d(0, 0, 10);
		glEnd();

        /* sekido */
        /*
		glBegin( GL_QUADS );
        glColor4f(0.6, 0.2, 0.2, 0.4);
        glVertex3d(-5, -5, 0);
		glVertex3d(-5, 5, 0);
		glVertex3d(5, 5, 0);
		glVertex3d(5, -5, 0);
		glEnd();
        */

		glBegin( GL_LINE_STRIP );
        glColor3d(0.7, 0.1, 0.2);
		for(j=0 ; j<i ; j++) {
			//glVertex3f( xi[j], yi[j], zi[j]);
			glVertex3f( xi[j], yi[j], zi[j]);
		}
		glEnd();

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
		//glTranslatef( xi[i-1], yi[i-1], zi[i-1] );
		glTranslatef( xi[i-1], yi[i-1], zi[i-1] );
        glColor3d(0.7, 0.1, 0.2);
		glutSolidSphere(0.1,30,30);
		glPopMatrix();
        //_glDisable(GL_DEPTH_TEST);
}

void SaveImage_PPM(const std::string& fname) { // save current screen buffe image to the ppm file
    int viewport[4];
    ::glGetIntegerv( GL_VIEWPORT, viewport);   // get location and size of viewport
    const unsigned int width = viewport[2];   // viewport width
    const unsigned int height = viewport[3];  //  viewport height
    void* image = malloc(3*width* height);  // allocate buffer (rgb)*width*height
    ::glPixelStorei(GL_PACK_ALIGNMENT, 1);
    ::glReadPixels(0, 0, width,height, GL_RGB, GL_UNSIGNED_BYTE, image);   // get image
    // write to file
    fout.open(fname.c_str(),std::ios::out);
    fout << "P3\n";   // ascii format
    fout << width << " " << height << "\n";
    fout << "255\n";  // range of the rgb data
    char* img = (char*)image;  
    for(unsigned int ih=0;ih<height;ih++){    
        for(unsigned int iw=0;iw<width;iw++){    
            unsigned int i = (height-1-ih)*width+iw;
            int r = (unsigned char)img[i*3+0];
            int g = (unsigned char)img[i*3+1];
            int b = (unsigned char)img[i*3+2];
            fout << r << " " << g << " " << b << "\n";
        }
    }
    fout.close();
}

void display(void){
		glClearColor( .0, .0, .0, .0);
		glClear( GL_COLOR_BUFFER_BIT );
//		glRotated(0.3, 0.8, 1.0, 0.6);
//      glRotated(0.5, 0.1, 1.0, 0.1);
		FreeFall();
        //char str[13];
        //sprintf(str, "out%.4d.ppm", count);
        //SaveImage_PPM(str);
        count++;
		glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            glRotated(5, 1.0, 0.0, 0.0);
            break;
        case 'X':
            glRotated(5, -1.0, 0.0, 0.0);
            break;
        case 'y':
            glRotated(5, 0.0, 1.0, 0.0);
            break;
        case 'Y':
            glRotated(5, 0.0, -1.0, 0.0);
            break;
        case 'z':
            glRotated(5, 0.0, 0.0, 1.0);
            break;
        case 'Z':
            glRotated(5, 0.0, 0.0, -1.0);
            break;
        case 'h':
            glTranslated(1.0, 0, 0);
            break;
        case 'j':
            glTranslated(0, -1.0, 0);
            break;
        case 'k':
            glTranslated(0, 1.0, 0);
            break;
        case 'l':
            glTranslated(-1.0, 0, 0);
            break;
        case 'q':
        case 'Q':
        case '\033':
            exit(0);
            break;
        default:
            break;
    }
}

void timer(int value) {
		glutTimerFunc(20 , timer , 0);
		i++;
		glutPostRedisplay();
}

void openGL(const char* file) {
		if ((fp3 = fopen(file, "r")) == NULL) {
			printf("cannot open file..");
			exit(0);
		}

		char data[100];
		int l = 0;

        while (fgets(data, 100, fp3)) {
            sscanf(data, "%f%f%f",&xi[l], &yi[l], &zi[l]);
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

        gluPerspective(30.0, 1, 1, 50.0);
        //gluPerspective(30.0, (double)width / (double)height, 1.0, 5.0);
//		glOrtho(-2.0, 2.0, -2.0, 2.0, -8.0, 8.0);
		//glTranslated(0.0, 0.0, -10.0);
		glTranslated(0, 0, -20.0);

//		gluOrtho2D( -1.7, 1.7, -1.7, 1.7 );
		glEnable(GL_LIGHTING);
        glDisable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, green);
        glutKeyboardFunc(keyboard);

//        glRotated(5*25, 1.0, 0.0, 0.0);
//        glRotated(5*3, 0.0, -1.0, 3.0);
		glutMainLoop();
}

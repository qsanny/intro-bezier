#include <iostream>
#include <new>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>

using namespace std ;

const float PI = 3.1415926535;

// variables globales pour OpenGL
int window,width,height;
const int NMAX = 100;
int N = 0;

int mp=-1,droite=0,gauche=0;

struct Point {
	float x,y;
	Point(float a=0, float b=0) {set(a,b);}
	void set(float a, float b) {x=a;y=b;}
};

Point P[NMAX];

const int NBEZ_MAX = 100;
Point bezierCurvePoints[NBEZ_MAX];

void Trace()
{
	glBegin(GL_POINTS);
	for (int i=0;i<N;i++){
		glVertex2f(P[i].x,P[i].y);
	}
	glEnd();
}

int computeBezierCoordinte(float p0, float p1, float p2, float p3, float t ){
	return pow((1-t), 3)*p0 + 3*t*pow((1-t), 2)*p1 + 3*pow(t, 2)*(1-t)*p2 + pow(t, 3)*p3;  
}

Point bezierCubique(Point p0, Point p1, Point p2, Point p3, float t)
{
	Point p;
	p.x = computeBezierCoordinte(p0.x, p1.x, p2.x, p3.x, t);
	p.y = computeBezierCoordinte(p0.y, p1.y, p2.y, p3.y, t);

	return p;
}

void traceCourbe()
{
	

	Point p0 = P[0];
	Point p1 = P[1];
	Point p2 = P[2];
	Point p3 = P[3];
		glBegin(GL_LINES);

	for(unsigned i=0; i<NBEZ_MAX;i++){
		float t = (float)i/(float)100;
		Point p = bezierCubique(p0, p1, p2, p3, t);
		// bezierCurvePoints[i] = p; 

		glColor3f(1.0,0.0,0.0);
		glPointSize(5.0);
		glVertex2f(p.x, p.y);

		cout<<i << " - " << t <<" SHOW point "<<p.x<<" "<<p.y<<"\n";
	}

		glEnd();

	

}



void
main_reshape(int width,  int height) 
{
	GLint viewport[4];
    glViewport(0, 0, width, height);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void
main_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glColor3f(0.0,1.0,0.0);
    glPointSize(3.0);
	glInitNames();
	glPushName(1);
	for (int i=0;i<N;i++){
		glLoadName(i);
		glBegin(GL_POINTS);
		glVertex2f(P[i].x,P[i].y);
		glEnd();	
	}
	if (N > 3){
		traceCourbe();
	}
	glutPostRedisplay();
	
    glutSwapBuffers();
}



void Mouse(int button, int state, int x, int y) {
	GLint viewport[4];

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glGetIntegerv(GL_VIEWPORT, viewport);

	if(button == GLUT_LEFT_BUTTON) {
		droite = 0; gauche = 1;
			glColor3f(0.0,0.0,1.0);
			glPointSize(3.0);
			glInitNames();
			glPushName(1);

			P[N].x = x;
			P[N].y = viewport[3]-y;

			glLoadName(N);
			glBegin(GL_POINTS);
				glVertex2f(P[N].x,P[N].y);
			glEnd();
			
			if(state == GLUT_UP) N++;
		
			glutPostRedisplay();
		}

	if(button == GLUT_RIGHT_BUTTON) {
		gauche = 0; droite = 1;
		if(state == GLUT_DOWN) {
			GLuint *selectBuf = new GLuint[200];
			GLuint *ptr;
			GLint hits;

			glSelectBuffer(200, selectBuf);
			glRenderMode(GL_SELECT);

			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPickMatrix(x, viewport[3]-y, 5.0, 5.0, viewport);
			glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);

			glColor3f(0.0,1.0,0.0);
			glPointSize(3.0);
			glInitNames();
			glPushName(1);

			for (int i = 0;i<N;i++) {
				glLoadName(i);
				glBegin(GL_POINTS);
				glVertex2f(P[i].x,P[i].y);
				glEnd();			
			}
				
			glPopMatrix();
			glFlush();

			hits = glRenderMode(GL_RENDER);
			if(hits) {
				ptr = (GLuint *)selectBuf;
				ptr += 3;
				mp = *ptr;
			}
		}

		if(state == GLUT_UP)
			mp = -1;

		main_reshape(viewport[2], viewport[3]);
		glutPostRedisplay();
	}
}

void Motion(int x, int y) {
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	
	if ((droite) &&(mp != -1)) {
		int i= mp;
		P[i].x = x;
		P[i].y = viewport[3]-y;
		Trace();
		glutPostRedisplay();
	}
}

int main (int argc, char** argv)
{

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);
    
    window = glutCreateWindow("Select");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
  	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);	

	glutPostRedisplay();  
    glutMainLoop();
    
    return 0;
}




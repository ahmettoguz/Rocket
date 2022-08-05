/*********
   CTIS164 - Template Source Program
----------
STUDENT : Ahmet Oðuz Ergin
SECTION : 3
HOMEWORK: 1
----------
PROBLEMS:    -
----------
ADDITIONAL FEATURES:
-> animated background (color transition of sky according to movement speed)
-> sky includes stars after reached black sky (state machine)
-> Have a chance to increase / decrease speed of the tv by button (You can speed up to examine faster)
-> Have a chance to change color by button
-> Off screen has color transition (makes more realistic)
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 750

#define TIMER_PERIOD  500. /speed_rocket// Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define ROCX -75 //-75
int rocy = -155; //-155


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

char open_screen = 0;
char activation_time = 1;
int skyy2;
int skyy1 = -225;
char skyy1status = 1;
char skyy2status = 0;
int rockr = 255, rockg = 255, rockb = 255;
int speed_rocket = 10;
double countstar = 0;
double cloudx = -325;
double cloudy = 70;
int rany = 0, ranx = 0;
int distance = 100;


//prototype
void onTimer(int v);

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void disp_tv_scr()
{
	//biggest rectangle color
	{
		glColor3ub(166, 255, 188);

		glRectf(-700, 375, 295, 215);
		glRectf(-700, 375, -445, -375);
		glRectf(-700, -225, 295, -375);
		glRectf(295, 375, 700, -375);
	}

	//up antenna
	{
		glLineWidth(5);
		glColor3ub(0, 0, 0);

		glBegin(GL_LINE_STRIP);
		glVertex2f(383, 324);
		glVertex2f(435, 261);
		glVertex2f(525, 356);
		glEnd();

		glLineWidth(1);

		//circles
		circle(383, 324, 10);
		circle(525, 356, 10);
	}

	//out big rectangle frame brown
	{
		//glRectf(-500, 260, 600, -275);// this cover the main screen so seperate it 
		glColor3ub(135, 54, 0);

		glRectf(-500, 260, 295, 215);
		glRectf(-500, 260, -445, -275);
		glRectf(-500, -225, 295, -275);
		glRectf(295, 260, 600, -275);
	}

	//mid big rectangle frame gray
	{
		//glRectf(-480, 240, 580, -255);
		glColor3ub(144, 157, 154);

		glRectf(-480, 240, 295, 215);
		glRectf(-480, 240, -445, -255);
		glRectf(-480, -225, 295, -255);
		glRectf(295, 240, 580, -255);
	}

	//small big rectangle  black screen
	{
		glColor3ub(0, 0, 0);

		glRectf(-450, 220, 295, 215);
		glRectf(-450, 220, -445, -230);
		glRectf(-450, -225, 295, -230);
		glRectf(295, 220, 300, -230);
	}

	//right side rectangle
	{
		//for speaker
		{
			//up
			{
				glLineWidth(4);
				glColor3ub(0, 0, 0);

				glBegin(GL_LINE_LOOP);
				glVertex2f(330, 210);
				glVertex2f(550, 210);
				glVertex2f(550, 120);
				glVertex2f(330, 120);
				glEnd();

				//triangles and circle
				{
					circle(440, 165, 15);

					//triangles
					{
						glLineWidth(1);
						glBegin(GL_TRIANGLES);
						glColor3ub(255, 143, 0);
						glVertex2f(330, 210);
						glColor3ub(0, 0, 0);
						glVertex2f(330, 120);
						glVertex2f(425, 165);

						glVertex2f(550, 210);
						glColor3ub(255, 143, 0);
						glVertex2f(550, 120);
						glColor3ub(0, 0, 0);
						glVertex2f(455, 165);
						glEnd();
					}
				}
			}

			//down
			{
				glLineWidth(4);
				glColor3ub(0, 0, 0);

				glBegin(GL_LINE_LOOP);
				glVertex2f(330, -220);
				glVertex2f(550, -220);
				glVertex2f(550, -130);
				glVertex2f(330, -130);
				glEnd();

				//triangles and circle
				{
					circle(440, -175, 15);

					//triangles
					{
						glLineWidth(1);
						glBegin(GL_TRIANGLES);
						glVertex2f(330, -130);
						glColor3ub(255, 143, 0);
						glVertex2f(330, -220);
						glColor3ub(0, 0, 0);
						glVertex2f(425, -175);

						glColor3ub(255, 143, 0);
						glVertex2f(550, -130);
						glColor3ub(0, 0, 0);
						glVertex2f(550, -220);

						glVertex2f(455, -175);
						glEnd();
					}
				}
			}
		}

		//main borders
		{
			glLineWidth(4);
			glColor3ub(131, 24, 24);

			glBegin(GL_LINE_LOOP);

			glVertex2f(320, 220);
			glVertex2f(320, -230);
			glVertex2f(560, -230);
			glVertex2f(560, 220);

			glEnd();
		}

		//buttons
		{
			//mainrec
			{
				glColor3ub(131, 24, 24);
				glBegin(GL_LINE_LOOP);

				//glVertex2f(328, 122);
				glVertex2f(330, 110);
				glVertex2f(550, 110);
				glVertex2f(550, -120);
				glVertex2f(330, -120);

				glEnd();
			}

			//button rec
			{
				//ON / OFF
				{
					glColor3ub(83, 83, 85);
					glRectf(340, 100, 540, 60);

					//frame
					glLineWidth(3);
					glColor3f(0, 0, 0);

					glBegin(GL_LINE_LOOP);
					glVertex2f(340, 100);
					glVertex2f(540, 100);
					glVertex2f(540, 60);
					glVertex2f(340, 60);
					glEnd();
				}

				//PAUSE / RESTART
				{
					glColor3ub(83, 83, 85);
					glRectf(340, 50, 540, 10);


					//frame
					glLineWidth(3);
					glColor3f(0, 0, 0);

					glBegin(GL_LINE_LOOP);
					glVertex2f(340, 50);
					glVertex2f(540, 50);
					glVertex2f(540, 10);
					glVertex2f(340, 10);
					glEnd();
				}

				//SPEED UP / DOWN
				{
					glColor3ub(83, 83, 85);
					glRectf(340, -10, 540, -50);


					//frame
					glLineWidth(3);
					glColor3f(0, 0, 0);

					glBegin(GL_LINE_LOOP);
					glVertex2f(340, -10);
					glVertex2f(540, -10);
					glVertex2f(540, -50);
					glVertex2f(340, -50);
					glEnd();

					//seperate
					glBegin(GL_LINES);
					glVertex2f(440, -10);
					glVertex2f(440, -50);
					glEnd();
				}

				//Change color
				{
					glColor3ub(83, 83, 85);
					glRectf(340, -60, 540, -100);


					//frame
					glLineWidth(3);
					glColor3f(0, 0, 0);

					glBegin(GL_LINE_LOOP);
					glVertex2f(340, -60);
					glVertex2f(540, -60);
					glVertex2f(540, -100);
					glVertex2f(340, -100);
					glEnd();
				}
			}

			//button text
			{
				glColor3f(1, 1, 1);
				vprint(410, 76, GLUT_BITMAP_9_BY_15, "ON / OFF");
				vprint(370, 26, GLUT_BITMAP_9_BY_15, "Pause / Restart");
				vprint(355, -33, GLUT_BITMAP_9_BY_15, "Speed Up  Speed Down");
				vprint(381, -83, GLUT_BITMAP_9_BY_15, "Change Color");
			}
		}
	}

	//CLOSE screen
	if (open_screen == 0)
	{
		glBegin(GL_QUAD_STRIP);
		//left side
		glColor3ub(28, 24, 20);
		glVertex2f(-445, 215);
		glVertex2f(-445, -225);
		glColor3ub(87, 95, 100);
		glVertex2f(-75, 215);
		glVertex2f(-75, -225);
		glColor3ub(28, 24, 20);
		glVertex2f(295, 215);
		glVertex2f(295, -225);

		glEnd();
	}

	//legs of the frames
	{
		glLineWidth(7);
		glColor3ub(0, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(-225, -275);
		glVertex2f(-275, -375);
		glVertex2f(325, -275);
		glVertex2f(375, -375);
		glEnd();

		glLineWidth(1);
	}

}

void disp_rocket()
{
	//top triangle
	{
		glColor3ub(rockr, rockg, rockb);
		glBegin(GL_TRIANGLES);
		glVertex2f(ROCX, rocy + 15);
		glVertex2f(ROCX - 70, rocy - 40);
		glVertex2f(ROCX + 70, rocy - 40);
		glEnd();
	}

	//middle rectangle
	{
		glRectf(ROCX - 70, rocy - 40, ROCX + 70, rocy - 185);
	}

	//down rectangle
	{
		glColor3ub(97, 106, 107);
		glRectf(ROCX - 60, rocy - 185, ROCX + 60, rocy - 200);
	}

	//righ and left triangles
	{
		glColor3ub(rockr, rockg, rockb);
		//left
		{
			glBegin(GL_TRIANGLES);
			glVertex2f(ROCX - 70, rocy - 105);
			glVertex2f(ROCX - 70, rocy - 175);
			glVertex2f(ROCX - 125, rocy - 175);
			glEnd();
		}

		//right
		{
			glBegin(GL_TRIANGLES);
			glVertex2f(ROCX + 70, rocy - 105);
			glVertex2f(ROCX + 70, rocy - 175);
			glVertex2f(ROCX + 125, rocy - 175);
			glEnd();
		}
	}


	// ornaments and frames
	{
		//ornamentals
		{
			//colors in detailed
			{
				//top red triangle
				{
					glColor3ub(255, 0, 0);
					glBegin(GL_TRIANGLES);
					glVertex2f(ROCX, rocy + 15);
					glVertex2f(ROCX - 37, rocy - 15);
					glVertex2f(ROCX + 37, rocy - 15);
					glEnd();
				}

				//left triangle
				{
					glBegin(GL_QUADS);
					glVertex2f(ROCX - 70, rocy - 135);
					glVertex2f(ROCX - 92, rocy - 135);
					glVertex2f(ROCX - 110, rocy - 155);
					glVertex2f(ROCX - 70, rocy - 155);
					glEnd();
				}

				//right triangle
				{
					glBegin(GL_QUADS);
					glVertex2f(ROCX + 70, rocy - 135);
					glVertex2f(ROCX + 92, rocy - 135);
					glVertex2f(ROCX + 110, rocy - 155);
					glVertex2f(ROCX + 70, rocy - 155);
					glEnd();
				}
			}

			//circles in body
			{
				//big
				glColor3ub(237, 51, 84);
				circle(ROCX, rocy - 80, 30);

				//middle
				glColor3ub(190, 119, 251);
				circle(ROCX, rocy - 80, 25);

				//little
				glColor3ub(128, 255, 255);
				circle(ROCX, rocy - 80, 20);

				//little
				glColor3ub(170, 130, 170);
				circle(ROCX, rocy - 80, 15);

				//little
				glColor3ub(128, 255, 255);
				circle(ROCX, rocy - 80, 10);

				//little
				glColor3ub(170, 130, 170);
				circle(ROCX, rocy - 80, 3);
			}

			//fires
			{
				glColor3ub(255, 80, 80);

				//right
				{
					glBegin(GL_TRIANGLES);
					glVertex2f(ROCX + 10, rocy - 210);
					glVertex2f(ROCX + 40, rocy - 265);
					glVertex2f(ROCX + 80, rocy - 240);
					glEnd();
				}

				//left
				{
					glBegin(GL_TRIANGLES);
					glVertex2f(ROCX - 10, rocy - 210);
					glVertex2f(ROCX - 40, rocy - 265);
					glVertex2f(ROCX - 80, rocy - 240);
					glEnd();
				}

				//mid
				{
					glBegin(GL_TRIANGLES);
					glVertex2f(ROCX, rocy - 220);
					glVertex2f(ROCX - 30, rocy - 280);
					glVertex2f(ROCX + 30, rocy - 280);
					glEnd();
				}

				//frames
				{
					glColor3ub(0, 0, 0);
					glLineWidth(3);
					//right
					{
						glBegin(GL_LINE_LOOP);
						glVertex2f(ROCX + 10, rocy - 210);
						glVertex2f(ROCX + 40, rocy - 265);
						glVertex2f(ROCX + 80, rocy - 240);
						glEnd();
					}

					//left
					{
						glBegin(GL_LINE_LOOP);
						glVertex2f(ROCX - 10, rocy - 210);
						glVertex2f(ROCX - 40, rocy - 265);
						glVertex2f(ROCX - 80, rocy - 240);
						glEnd();
					}

					//mid
					{
						glBegin(GL_LINE_LOOP);
						glVertex2f(ROCX, rocy - 220);
						glVertex2f(ROCX - 30, rocy - 280);
						glVertex2f(ROCX + 30, rocy - 280);
						glEnd();
					}

					glLineWidth(1);

				}

				//bubles
				{
					glColor3ub(255, 255, 14);
					//left
					circle(ROCX - 70, rocy - 270, 12);
					//right
					circle(ROCX + 70, rocy - 270, 12);
					//mid
					circle(ROCX + 0, rocy - 300, 15);

					//wires
					glColor3ub(0, 0, 0);
					glLineWidth(3);
					circle_wire(ROCX - 70, rocy - 270, 12);
					circle_wire(ROCX + 70, rocy - 270, 12);
					circle_wire(ROCX + 0, rocy - 300, 15);
					glLineWidth(1);
				}
			}
		}

		//frames
		{
			glColor3ub(0, 0, 0);
			glLineWidth(3);

			//top triangle
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(ROCX, rocy + 15);
				glVertex2f(ROCX - 70, rocy - 40);
				glVertex2f(ROCX + 70, rocy - 40);
				glEnd();
			}

			//middle rectangle
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(ROCX - 70, rocy - 40);
				glVertex2f(ROCX + 70, rocy - 40);
				glVertex2f(ROCX + 70, rocy - 185);
				glVertex2f(ROCX - 70, rocy - 185);
				glEnd();
			}

			//down rectangle
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(ROCX - 60, rocy - 185);
				glVertex2f(ROCX + 60, rocy - 185);
				glVertex2f(ROCX + 60, rocy - 200);
				glVertex2f(ROCX - 60, rocy - 200);
				glEnd();

				//Lýnes
				{
					glLineWidth(4);
					glBegin(GL_LINES);
					for (int a = ROCX - 60; a <= ROCX + 60; a += 12)
					{
						glVertex2f(a, rocy - 185);
						glVertex2f(a, rocy - 200);
					}
					glEnd();
					glLineWidth(3);
				}
			}

			//left  triangle
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(ROCX - 70, rocy - 105);
				glVertex2f(ROCX - 70, rocy - 175);
				glVertex2f(ROCX - 125, rocy - 175);
				glEnd();
			}

			//right  triangle
			{
				glBegin(GL_LINE_LOOP);
				glVertex2f(ROCX + 70, rocy - 105);
				glVertex2f(ROCX + 70, rocy - 175);
				glVertex2f(ROCX + 125, rocy - 175);
				glEnd();
			}

			//detailed triangle
			{
				glLineWidth(2);
				glBegin(GL_LINES);
				glVertex2f(ROCX - 37, rocy - 15);
				glVertex2f(ROCX + 37, rocy - 15);
				glEnd();
			}

			//detailed left right triangle
			{
				//left triangle
				{
					glBegin(GL_LINE_LOOP);
					glVertex2f(ROCX - 70, rocy - 135);
					glVertex2f(ROCX - 92, rocy - 135);
					glVertex2f(ROCX - 110, rocy - 155);
					glVertex2f(ROCX - 70, rocy - 155);
					glEnd();
				}

				//right triangle
				{
					glBegin(GL_LINE_LOOP);
					glVertex2f(ROCX + 70, rocy - 135);
					glVertex2f(ROCX + 92, rocy - 135);
					glVertex2f(ROCX + 110, rocy - 155);
					glVertex2f(ROCX + 70, rocy - 155);
					glEnd();
				}

			}

			//mid rectangle
			{
				glBegin(GL_LINES);
				glVertex2f(ROCX, rocy - 110);
				glVertex2f(ROCX, rocy - 185);
				glEnd();

				//CýRCLES
				{
					circle(ROCX, rocy - 140, 5);
					circle(ROCX, rocy - 155, 7);
					circle(ROCX, rocy - 170, 5);
				}
			}

			glLineWidth(1);
		}
	}
}

void display_bakcg()
{
	//skys
	{	//sky1
		{
			glBegin(GL_QUADS);

			if (skyy1status == 1)//white
				glColor3f(1, 1, 1);
			if (skyy1status == 2)
				glColor3f(0, 0, 0);//black

			glVertex2f(-445, skyy1);
			glVertex2f(295, skyy1);

			if (skyy1status == 1)//blue
				glColor3ub(0, 255, 255);
			if (skyy1status == 2)//violet
				glColor3ub(205, 159, 215);
			if (skyy1status == 3) //red
				glColor3ub(224, 92, 92);
			if (skyy1status == 4) //pink
				glColor3ub(250, 118, 190);


			glVertex2f(295, skyy1 + 500);
			glVertex2f(-445, skyy1 + 500);
			glEnd();
		}

		//sky2
		{
			glBegin(GL_QUADS);

			if (skyy2status == 1) //blue
				glColor3ub(0, 255, 255);

			if (skyy2status == 2) //violate
				glColor3ub(205, 159, 215);

			if (skyy2status == 3) //red
				glColor3ub(224, 92, 92);

			if (skyy2status == 4) //pink
				glColor3ub(250, 118, 190);




			glVertex2f(-445, skyy2);
			glVertex2f(295, skyy2);

			if (skyy2status == 1 || skyy2status == 2 || skyy2status == 3 || skyy2status == 4)
				glColor3ub(0, 0, 0); //black



			glVertex2f(295, skyy2 + 500);
			glVertex2f(-445, skyy2 + 500);
			glEnd();
		}
	}

	//starts

	if (skyy1status == 2 || skyy1status == 3 || skyy1status == 4 || skyy2status == 4)
	{
		if (int(countstar) % 50 == 0)
		{
			ranx = rand() % ((300 + 1) + 440) - 440;
			rany = rand() % ((165 + 1) + 165) - 165;
		}
		glColor3f(1, 1, 1);

		vprint2(ranx - 200, rany + 91, 1, "*");
		vprint2(ranx - 300, rany + 123, 0.3, ".");
		vprint2(ranx - 400, rany + 321, 1, ".");
		vprint2(ranx - 500, rany + 91, 1, "*");
		vprint2(ranx + -16, rany - 100, 0.3, ".");
		vprint2(ranx + 150, rany - 250, 2.4, ".");
		vprint2(ranx + 200, rany + 0, 1, ".");
		vprint2(ranx - 500, rany - 150, 1, ".");
		vprint2(ranx + 100, rany - 82, 1, "*");
		vprint2(ranx + 200, rany - 197, 0.6, "+");

		vprint2(ranx + 500, rany - 94, 1, ".");


		glColor3f(0, 0, 0);
		vprint2(ranx + 500, rany + 90, 1, "*");
		vprint2(ranx - 100, rany - 30, 1, ".");
		vprint2(ranx - 200, rany - 60, 1, ".");
		vprint2(ranx - 300, rany - 45, 0.3, "*");
		vprint2(ranx - 400, rany - 200, 2.4, "*");
		vprint2(ranx + 100, rany + 50, 1, ".");
		vprint2(ranx + 200, rany + 60, 2, "*");
		vprint2(ranx + 300, rany + 70, 0.3, "*");
		vprint2(ranx + 400, rany + 80, 2.4, "*");
		vprint2(ranx - 100, rany + 56, 0.4, "+");
		vprint2(ranx + 300, rany - 35, 0.3, "+");
		vprint2(ranx + 400, rany - 167, 1, "+");

	}

	//clouds
	if (skyy1status == 1 || skyy1status == 2)
	{
		//left
		{
			//wire
			glColor3f(0, 0, 0);
			circle(cloudx, cloudy, 29);
			circle(cloudx - 30, cloudy, 24);
			circle(cloudx + 30, cloudy, 24);
			circle(cloudx - 19, cloudy + 15, 24);
			circle(cloudx + 19, cloudy + 15, 24);
			circle(cloudx, cloudy + 22, 19);

			//cloud
			glColor3f(1, 1, 1);
			circle(cloudx, cloudy, 25);
			circle(cloudx - 30, cloudy, 20);
			circle(cloudx + 30, cloudy, 20);
			circle(cloudx - 19, cloudy + 15, 20);
			circle(cloudx + 19, cloudy + 15, 20);
			circle(cloudx, cloudy + 22, 15);
		}

		//left buttom
		{
			//wire
			glColor3f(0, 0, 0);
			circle(cloudx - 30, cloudy - 80, 29);
			circle(cloudx - 30 - 30, cloudy - 80, 24);
			circle(cloudx - 30 + 30, cloudy - 80, 24);
			circle(cloudx - 30 - 19, cloudy - 80 + 15, 24);
			circle(cloudx - 30 + 19, cloudy - 80 + 15, 24);
			circle(cloudx - 30, cloudy - 80 + 22, 19);

			//cloud
			glColor3f(1, 1, 1);
			circle(cloudx - 30, cloudy - 80, 25);
			circle(cloudx - 30 - 30, cloudy - 80, 20);
			circle(cloudx - 30 + 30, cloudy - 80, 20);
			circle(cloudx - 30 - 19, cloudy - 80 + 15, 20);
			circle(cloudx - 30 + 19, cloudy - 80 + 15, 20);
			circle(cloudx - 30, cloudy - 80 + 22, 15);
		}


		//right
		{
			//wire
			glColor3f(0, 0, 0);
			circle(cloudx + 450, cloudy - 30, 29);
			circle(cloudx + 450 - 30, cloudy - 30, 24);
			circle(cloudx + 450 + 30, cloudy - 30, 24);
			circle(cloudx + 450 - 19, cloudy - 30 + 15, 24);
			circle(cloudx + 450 + 19, cloudy - 30 + 15, 24);
			circle(cloudx + 450, cloudy - 30 + 22, 19);

			//cloud
			glColor3f(1, 1, 1);
			circle(cloudx + 450, cloudy - 30, 25);
			circle(cloudx + 450 - 30, cloudy - 30, 20);
			circle(cloudx + 450 + 30, cloudy - 30, 20);
			circle(cloudx + 450 - 19, cloudy - 30 + 15, 20);
			circle(cloudx + 450 + 19, cloudy - 30 + 15, 20);
			circle(cloudx + 450, cloudy - 30 + 22, 15);
		}

		//right mid
		{
			//wire
			glColor3f(0, 0, 0);
			circle(cloudx + 550, cloudy - 30 - 30, 29);
			circle(cloudx + 550 - 30, cloudy - 30 - 30, 24);
			circle(cloudx + 550 + 30, cloudy - 30 - 30, 24);
			circle(cloudx + 550 - 19, cloudy - 30 - 30 + 15, 24);
			circle(cloudx + 550 + 19, cloudy - 30 - 30 + 15, 24);
			circle(cloudx + 550, cloudy - 30 - 30 + 22, 19);

			//cloud
			glColor3f(1, 1, 1);
			circle(cloudx + 550, cloudy - 30 - 30, 25);
			circle(cloudx + 550 - 30, cloudy - 30 - 30, 20);
			circle(cloudx + 550 + 30, cloudy - 30 - 30, 20);
			circle(cloudx + 550 - 19, cloudy - 30 - 30 + 15, 20);
			circle(cloudx + 550 + 19, cloudy - 30 - 30 + 15, 20);
			circle(cloudx + 550, cloudy - 30 - 30 + 22, 15);
		}

		//right buttom
		{
			//wire
			glColor3f(0, 0, 0);
			circle(cloudx + 500 - 30, cloudy - 30 - 90, 29);
			circle(cloudx + 500 - 30 - 30, cloudy - 30 - 90, 24);
			circle(cloudx + 500 - 30 + 30, cloudy - 30 - 90, 24);
			circle(cloudx + 500 - 30 - 19, cloudy - 30 - 90 + 15, 24);
			circle(cloudx + 500 - 30 + 19, cloudy - 30 - 90 + 15, 24);
			circle(cloudx + 500 - 30, cloudy - 30 - 90 + 22, 19);

			//cloud
			glColor3f(1, 1, 1);
			circle(cloudx + 500 - 30, cloudy - 30 - 90, 25);
			circle(cloudx + 500 - 30 - 30, cloudy - 30 - 90, 20);
			circle(cloudx + 500 - 30 + 30, cloudy - 30 - 90, 20);
			circle(cloudx + 500 - 30 - 19, cloudy - 30 - 90 + 15, 20);
			circle(cloudx + 500 - 30 + 19, cloudy - 30 - 90 + 15, 20);
			circle(cloudx + 500 - 30, cloudy - 30 - 90 + 22, 15);

		}
	}
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//display background
	display_bakcg();

	//display rocket
	disp_rocket();

	//display tv screen frame permanently
	disp_tv_scr();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.
	//conversion
	x = x - winWidth / 2;
	y = winHeight / 2 - y;

	//to check ON / OFF
	{
		if (stat == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
			if (x >= 340 && x <= 540 && y <= 100 && y >= 60)
				if (open_screen == 1)
					open_screen = 0;
				else
					open_screen = 1;
	}

	//to check Change Color
	{
		if (stat == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
			if (x >= 340 && x <= 540 && y <= -60 && y >= -100)
			{
				rockr = rand() % ((255 + 1) - 0) + 0;
				rockb = rand() % ((255 + 1) - 0) + 0;
				rockg = rand() % ((255 + 1) - 0) + 0;
			}
	}

	//STOP / RESTART
	{
		if (stat == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
			if (x >= 340 && x <= 540 && y <= 50 && y >= 10)
				if (activation_time == 0)
				{
					activation_time = 1;
					glutTimerFunc(TIMER_PERIOD, onTimer, 0);
				}
				else
					activation_time = 0;
	}

	//SPEED UP / DOWN
	{
		if (stat == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			if (x >= 340 && x <= 440 && y <= -10 && y >= -50 && speed_rocket < 24)
				speed_rocket++;
			if (x >= 440 && x <= 540 && y <= -10 && y >= -50 && speed_rocket > 4)
				speed_rocket--;
		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.




	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	// Write your codes here.

	if (activation_time == 1)
	{
		if (skyy2 == -225)
		{
			skyy1 = 275;
			if (skyy1status == 1)
				skyy1status = 2;
			else if (skyy1status == 2)
				skyy1status = 3;
			else if (skyy1status == 3)
				skyy1status = 4;
			else if (skyy1status == 4)
				skyy1status = 1;

		}

		if (skyy1 == -225)
		{
			skyy2 = 275;

			if (skyy2status == 0)
				skyy2status = 1;
			else if (skyy2status == 1)
				skyy2status = 2;
			else if (skyy2status == 2)
				skyy2status = 3;
			else if (skyy2status == 3)
				skyy2status = 4;
			else if (skyy2status == 4)
			{
				cloudx = -325;
				cloudy = 70;
				skyy2status = 1;
			}
		}
		if (skyy1status == 1 || skyy1status == 2)
			cloudy -= 0.4;
		//cloudx -= 0.5;

		skyy1 -= 1;
		skyy2 -= 1;

		if (rocy >= 560)
			rocy = -300;
		rocy += speed_rocket;
		countstar++;
		if (countstar == 100000)
			countstar = 0;
		glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(0));
}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(75, 50);
	glutCreateWindow("Ahmet Oðuz Ergin - Live Stream of Astronaut Ahmet");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
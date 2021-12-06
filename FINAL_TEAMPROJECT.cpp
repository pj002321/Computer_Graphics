#include<glew.h>
#include<freeglut.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/ext.hpp>
#include<glm/glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<math.h>
#include<fstream>
#include<string>
#include"readTriangleObj.h"


#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
using namespace std;

//shader func
void makeVertexShader();
void makeFragmentShader();
void makeShaderID();
void InitBuffer();
void initTexture();

//call_back
void timer(int value);
void MainView();
void Resize(int w, int h);
void keyboardCall(unsigned char key, int x, int y);


//func
void drawPlane();
void ObjList();
void drawscene();
void LocationSnow();

GLuint fragmentShader;
GLuint modelvertexShader;
GLuint shaderID;

char* vertexSource;
char* fragmentSource;

int Wwidth = 0;
int Wheight = 0;

//Struck

struct Camera
{
	float C_x = 2.0f;
	float C_y = 3.0f;
	float C_z = 9.0f;

}Camerapos;


struct Angle
{
	float angle = 0.0f;
	float anglex = 0.0f;
	float angley = 0.0f;
	float AngleTrap = 0.0f;
	float Radian = 0.0f;
	//---------------------
	float AngleRevolx = 45.0f;
	float AngleRevoly = 45.0f;
	//----------------------
	float anglecamera = 160.0f;
	float anglecamera2 = 0.0f;
	float ObjAngle = 20.0f;
	//------------------
	float LightRadian = 210.0f;
	//------------------
	float BodyAngle = 0.0f;
	float ArmAngle = 45.0f;
	float LegAngle = 0.0f;
	float MainSwing1 = 0.0f;
	float MainSwing2 = 0.0f;
	float StageAngle = 0.0f;
	float StageAngle2 = 0.0f;
}AngleList;


//scale
struct Scale
{
	float X = 0.0f;
	float Y = 4.5f;
	float Z = 6.5f;
	//-------------
	float x = 2.0f;
	float y = 2.0f;
	float z = 2.0f;
	//-------------
	float Mx = 0.0f;
	float My = 0.0f;
	float Mz = 0.0f;
	//--------------
	float ALx = 0.1f;
	float ALy = 0.3f;
	float ALz = 0.1f;

}Scalepos;

//trans
struct Transration
{
	float T_r1x = 4.0f;
	float T_r1y = 0.0f;
	float T_r1z = -4.0f;
	//---------------
	float T_r2x = -4.0f;
	float T_r2y = 0.0f;
	float T_r2z = -4.0f;
	//--------------
	float T_r3x = -2.0f;
	float T_r3y = 0.0f;
	float T_r3z = 0.0f;
	//----------------
	float T_r4x = 2.0f;
	float T_r4y = 0.0f;
	float T_r4z = 0.0f;
	//------------------
	float T_x = 0.0f;
	float T_y = 0.8f;
	float T_z = 0.0f;
	//---------------
	float T_StageX = 0.0f;
	float T_StageY = 0.0f;
	float T_StageZ = 0.0f;
	//----------------
	float T_Trapx = 0.0f;
	float T_Trapy = 0.6f;
	float T_Trapz = 0.0f;
	//---------------
	float T_Bodyx = 0.0f;
	float T_Bodyy = 0.9f;
	float T_Bodyz = -2.0f;
	//----------------
	float T_ArmLegx = 0.0f;
	float T_ArmLegy = 0.77f;
	float T_ArmLegz = -0.0f;

}TransList;

struct Snow {
	float x;
	float y;
	float z;
};
Snow SLocation[80];
Snow SaveLocation[80];
Snow SnowSpeed[80];

struct Trap {
	float x;
	float y;
	float z;
};
 Trap radomcorail;


GLuint VAO[20];
GLuint VBO[40];

vector<glm::vec4> Vertex[15];
vector<glm::vec4> Nomal[15];
vector<glm::vec2> Texture[15];
unsigned int texture[10];

float snowx = 0.0f;
float snowy = 5.8f;
float snowz = 0.0f;
unsigned int pyramidtexture;
float angle = 0.0f;
float cameraRevolu = 0.0f;
float lightRevoluAngle = 0.0f;
bool change = false;
bool Rotate = true;
bool ScreenRotate = false;
bool Opening = false;
int Mainswingchk = 1;
bool MainswingAngle1 = true;
bool MainswingAngle2 = false;
bool TrapAction = true;
glm::vec3 objC = glm::vec3(0, 0, 0);

glm::vec3 cameraPos = glm::vec3(1.0f, 3.0f, 8.0f);
glm::vec3 lightPos = glm::vec3(0, 3.0f, 2.5f);
glm::vec3 lightColor = glm::vec3(1.4f, 1.3f, 1.3f);
glm::vec3 Cameraposdir = glm::vec3(0.0f);
glm::vec3 Cameradir = glm::vec3(0.0f);
float Buildingx[8] = { -4.0f,3.5f,-4.0f,3.5f,-4.0f,3.5f,-4.0f,3.5f };
float Buildingz[8] = { -2.0f,-2.0f,2.5f,2.5f,-3.0f,-3.0f,3.5f,3.5f };

glm::vec3 lightColorKind[4] = {
	glm::vec3(0.7f, 0.7f, 0.7f),
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1)
};
int selectLightColor = 0;


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Wwidth, Wheight);
	glutCreateWindow("texture example");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		cerr << "fail Initialize" << endl;
	else cout << "Initialize" << endl;
	LocationSnow();
	makeShaderID();
	ObjList();
	InitBuffer();
	initTexture();
	glutDisplayFunc(MainView);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(keyboardCall);
	glutTimerFunc(1, timer, 1);
	glutMainLoop();
}
bool snowcheck = true;
void timer(int value)
{
	if (ScreenRotate)
	{
		cameraRevolu -= 2.0f;
	}
	if (Rotate == true)
	{
		angle += 3.0f;
	}
	if (snowcheck == true)
	{
		for (int i = 0; i < 80; i++) {
			SLocation[i].y += SnowSpeed[i].y;
			if (SLocation[i].y >= 6.0f) {
				SLocation[i].y = SaveLocation[i].y;
			}

		}
	}

	if (Mainswingchk)
	{

		if (MainswingAngle1 == true) {
			AngleList.MainSwing1 += 2.0f;
			if (AngleList.MainSwing1 == 20.0f) {
				MainswingAngle1 = false;
			}
		}
		if (MainswingAngle1 == false) {
			AngleList.MainSwing1 -= 2.0f;
			if (AngleList.MainSwing1 == -20.0f) {
				MainswingAngle1 = true;
			}

		}
		if (MainswingAngle2 == true) {
			AngleList.MainSwing2 += 2.0f;

			if (AngleList.MainSwing2 == 20.0f) {
				MainswingAngle2 = false;
			}
		}
		if (MainswingAngle2 == false) {
			AngleList.MainSwing2 -= 2.0f;
			if (AngleList.MainSwing2 == -20.0f) {
				MainswingAngle2 = true;
			}

		}
	}

	if (TrapAction==true)
	{
		AngleList.AngleTrap -= 5.0f;
		TransList.T_Trapz -= 0.03f;
		 
		if (TransList.T_Trapz+16.0f<-6.0f)
		{
			TransList.T_Trapz = 0.0f;
			TransList.T_Trapx = radomcorail.x;
		
		}
	}


	glutPostRedisplay();
	glutTimerFunc(17, timer, value);
}

void MainView()
{
	glClearColor(0.9989, 0.9989, 0.9989, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	{
		glBindVertexArray(VAO[0]);
		unsigned int StartGroundBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(StartGroundBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
		glm::mat4 StartGround= glm::mat4(1.0f);
		unsigned int StartGroundNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(StartGroundNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		unsigned int cameraViewLocation = glGetUniformLocation(shaderID, "viewTransform");
		glUniformMatrix4fv(cameraViewLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		unsigned int cameraPosLocation = glGetUniformLocation(shaderID, "cameraPos");
		glUniform3fv(cameraPosLocation, 1, glm::value_ptr(glm::vec3(0, 0, 1)));
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		StartGround = glm::translate(StartGround, glm::vec3(0, 0, -1));
		StartGround = glm::rotate(StartGround, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		StartGround = glm::scale(StartGround, glm::vec3(2, 2, 2));
		unsigned int backgroundMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(backgroundMatrixLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		glDrawArrays(GL_TRIANGLES, 0, Vertex[0].size());
	}

	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glUseProgram(shaderID);
	//------------------------------camera---------------------------------------------

	glm::vec3 ObjectCamerapos = glm::vec3(Camerapos.C_x-TransList.T_Bodyx, Camerapos.C_y + TransList.T_Bodyy, Camerapos.C_z - TransList.T_Bodyz);
	glm::mat4 CameraSpacepos = glm::mat4(1.0f);
	CameraSpacepos = glm::rotate(CameraSpacepos, glm::radians(AngleList.anglecamera), glm::vec3(0.0f, 1.0f, 0.0f));//°øÀü
	glm::vec3 Cameraposdir = glm::vec3(CameraSpacepos*glm::vec4(ObjectCamerapos, 1));
	glm::vec3 ObjectCameraPicking = glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy, TransList.T_Bodyz);
	glm::vec3 ObjectCameradir = Cameraposdir - ObjectCameraPicking;
	glm::vec3 Up_y = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 CameraSpacedir = glm::mat4(1.0f);
	CameraSpacedir = glm::rotate(CameraSpacedir, glm::radians(AngleList.anglecamera2), glm::vec3(0.0f, 1.0f, 0.0f));//ÀÚÀü 
	glm::vec3 Cameradir =glm::vec3( CameraSpacedir * glm::vec4(-ObjectCameradir, 1));

	glm::vec3 Crosspos = glm::normalize(glm::cross(Up_y, glm::normalize(Cameradir)));
	glm::vec3 Cross = glm::cross(glm::normalize(Cameradir), Crosspos);

	glm::mat4 ObjView = glm::mat4(1.0f);
	ObjView = glm::lookAt(Cameraposdir, Cameradir+ Cameraposdir, Cross);
	unsigned int ObjcameraViewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(ObjcameraViewLocation, 1, GL_FALSE, glm::value_ptr(ObjView));

	glm::vec3 CameraView;
	CameraView = glm::vec3(Cameraposdir);
	unsigned int ViewPositionLocation = glGetUniformLocation(shaderID, "camerapos");
	glUniform3fv(ViewPositionLocation, 1, glm::value_ptr(CameraView));

	//--------¿ø±Ù
	glm::mat4 Proj = glm::mat4(1.0f);
	Proj = glm::perspective(glm::radians(60.0f), (float)Wwidth / Wheight, 0.1f, 100.0f);
	unsigned int ModelProjLocation = glGetUniformLocation(shaderID, "projectionTransform");
	glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &Proj[0][0]);


	//--------Á¶¸í
	glm::mat4 LightPosition = glm::mat4(1.0f);
	LightPosition = glm::rotate(LightPosition, glm::radians(AngleList.LightRadian), glm::vec3(0.0f, 1.0f, 0.0f));
	LightPosition = glm::translate(LightPosition, glm::vec3(Scalepos.X, Scalepos.Y, Scalepos.Z));
	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "LightPos");
	glUniform3fv(lightPosLocation, 1, glm::value_ptr(glm::vec3(0, 0, 0)));
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "LightColor");
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
	unsigned int LightTransformLocation = glGetUniformLocation(shaderID, "LightTransform");
	glUniformMatrix4fv(LightTransformLocation, 1, GL_FALSE, glm::value_ptr(LightPosition));


	drawscene();
	glutSwapBuffers();
}

void Resize(int w, int h)
{
	glViewport(0,0, w, h);
	Wwidth = w;
	Wheight = h;
}

void keyboardCall(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'w':
		Mainswingchk = 1;
		AngleList.BodyAngle = 180.0f;
		AngleList.LegAngle = 0.0f;
		TransList.T_Bodyz += 0.06f;
		TransList.T_ArmLegz += 0.06f;
		break;

	case's':
		Mainswingchk =1;
		AngleList.BodyAngle = 270.0f;
		AngleList.LegAngle = 0.0f;
		TransList.T_Bodyz -= 0.06f;
		TransList.T_ArmLegz -= 0.06f;
		break;

	case'a':
		Mainswingchk = 2;
		AngleList.BodyAngle = 0.0f;
		AngleList.LegAngle = 180.0f;
		TransList.T_Bodyx += 0.04f;
		TransList.T_ArmLegx += 0.04f;
		break;

	case'd':
		Mainswingchk = 2;
		AngleList.BodyAngle = 0.0f;
		AngleList.LegAngle = 180.0f;
		TransList.T_Bodyx -= 0.04f;
		TransList.T_ArmLegx -= 0.04f;
		break;

	case'z':
		cameraPos.y += 0.1f;
		break;

	case'Z':
		AngleList.anglecamera -= 3.0f;
		break;

	case'c':
		selectLightColor++;
		lightColor = lightColorKind[selectLightColor % 4];
		break;
	case'r':
		AngleList.ArmAngle += 1.0f;
		break;
	case'R':
		lightRevoluAngle -= 1.0f;
		break;
	case'k':
		lightPos.z += 0.2f;
		break;
	case'l':
		lightPos.z -= 0.2f;
		break;
	case 'y':
		ScreenRotate = !ScreenRotate;
		break;
	case'q':
		glutLeaveMainLoop();
		break;
	case 'n': //special
		AngleList.StageAngle-=5.0f;
		AngleList.StageAngle2 -= 5.0f;
		break;
	case 'N': //special
		AngleList.StageAngle += 5.0f;
		AngleList.StageAngle2 += 5.0f;
		break;



	}
	glutPostRedisplay();
}


char* filetobuf(const string name)
{
	vector<char> tempFile;
	ifstream in(name, ios::binary);
	char temp;
	while (true) {
		noskipws(in);
		in >> temp;
		if (in.eof()) {
			tempFile.push_back(0);
			break;
		}
		else
			tempFile.push_back(temp);
	}
	char* addr = new char[tempFile.size()];
	for (int i = 0; i < tempFile.size(); i++) {
		addr[i] = tempFile[i];
	}
	return addr;
}

void makeVertexShader()
{
	vertexSource = filetobuf("VertexTextureShader.glsl");
	modelvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(modelvertexShader, 1, &vertexSource, NULL);
	glCompileShader(modelvertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(modelvertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(modelvertexShader, 512, NULL, errorLog);
		cerr << "VERTEXSHADER ERROR: " << errorLog << endl;
	}
}

void makeFragmentShader()
{
	fragmentSource = filetobuf("fragmentBlendShader.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "FRAGMENT SHADER ERROR: " << errorLog << endl;
	}
}

void makeShaderID()
{
	makeVertexShader();
	makeFragmentShader();

	shaderID = glCreateProgram();

	glAttachShader(shaderID, modelvertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);
	GLint result;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	GLchar errorLog[512];
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		cerr << "ShaderID0 Program ERROR: " << errorLog << endl;
	}

	glDeleteShader(modelvertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(shaderID);
}
void LocationSnow() {
	for (int i = 0; i < 80; i++) {
		random_device rd;
		default_random_engine dre(rd());
		uniform_real_distribution<>ScaleXZ(-6.0, 6.0);
		uniform_real_distribution<>LoctionY(0.0, 7.0);
		uniform_real_distribution<>Speed(0.02, 0.05);
		uniform_real_distribution<>RandomX(-4.0, 4.0);
		float Sx = ScaleXZ(dre);
		float Sz = ScaleXZ(dre);
		float Sy = LoctionY(dre);
		float SpeedY = Speed(dre);
		float RX = RandomX(dre);

		radomcorail.x = RX;
		SLocation[i].x = Sx;
		SLocation[i].y = Sy;
		SLocation[i].z = Sz;

		SaveLocation[i].x = Sx;
		SaveLocation[i].y = Sy;
		SaveLocation[i].z = Sz;

		SnowSpeed[i].y = SpeedY;
	}
}

void InitBuffer()
{
	glGenVertexArrays(14, VAO);

	glBindVertexArray(VAO[0]);
	glGenBuffers(3, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Vertex[0].size(), &Vertex[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Nomal[0].size(), &Nomal[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * Texture[0].size(), &Texture[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);

	for (int i = 1; i < 5; i++)
	{
	glBindVertexArray(VAO[i]);
	glGenBuffers(3, &VBO[i*3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[i * 3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Vertex[i].size(), &Vertex[i][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[i * 3+1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Nomal[i].size(), &Nomal[i][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[i * 3+2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * Texture[i].size(), &Texture[i][0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);

	}

	for (int i = 5; i < 13; i++)
	{
	glBindVertexArray(VAO[i]);
	glGenBuffers(3, &VBO[3*i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3 * i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Vertex[i].size(), &Vertex[i][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3 * i+1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Nomal[i].size(), &Nomal[i][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3 * i+2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * Texture[i].size(), &Texture[i][0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);

	}

}

void ObjList()
{
	readTriangleObj("plane3.obj", Vertex[0], Texture[0], Nomal[0]);
	readTriangleObj("plane3.obj", Vertex[9], Texture[9], Nomal[9]);


	readTriangleObj("cube3_.obj", Vertex[2], Texture[2], Nomal[2]);
	readTriangleObj("Sphere2.obj", Vertex[3], Texture[3], Nomal[3]);
	readTriangleObj("plane3.obj", Vertex[4], Texture[4], Nomal[4]);

	readTriangleObj("Sphere2.obj", Vertex[1], Texture[1], Nomal[1]);
	for (int i = 5; i < 9; i++)
	{
	readTriangleObj("Sphere2.obj", Vertex[i], Texture[i], Nomal[i]);
	}
	for (int i = 10; i < 13; i++)
	{
		readTriangleObj("Sphere2.obj", Vertex[i], Texture[i], Nomal[i]);
	}


}

void drawscene()
{
	glUseProgram(shaderID);

	for (int i = 0; i < 80; i++)
	{
		glBindVertexArray(VAO[3]);
		unsigned int snowBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(snowBlendCheck, 0);
		glm::mat4 Snow = glm::mat4(1.0f);
		Snow = glm::translate(Snow, glm::vec3(SLocation[i].x, SLocation[i].y, SLocation[i].z));
		Snow = glm::scale(Snow, glm::vec3(0.03f, 0.03f, 0.03f));
		unsigned int SnowLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(SnowLocation, 1, GL_FALSE, glm::value_ptr(Snow));
		glm::mat4 SnowNormalmodel = glm::mat4(1.0f);
		SnowNormalmodel = glm::translate(SnowNormalmodel, glm::vec3(SLocation[i].x, SLocation[i].y, SLocation[i].z));
		unsigned int SnowNormalmodelLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(SnowNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(SnowNormalmodel));
		unsigned int SnowColorLocation = glGetUniformLocation(shaderID, "objColor");
		glUniform3f(SnowColorLocation, 1	,0.388235,	0.278431);
		glDrawArrays(GL_TRIANGLES, 0, Vertex[3].size());
	}
	//
	//for (size_t i = 0; i < 8; i++)
	//{

	//glBindVertexArray(VAO[2]);
	//unsigned int BUILDINGBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	//glUniform1i(BUILDINGBlendCheck, 2);
	//glm::mat4 BUILDING = glm::mat4(1.0f);
	//BUILDING = glm::translate(BUILDING, glm::vec3(Buildingx[i], 0.6f, Buildingz[i]));
	//BUILDING = glm::scale(BUILDING, glm::vec3(1.0f, 3.0f, 1.0f));
	//unsigned int BUILDINGTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	//glUniformMatrix4fv(BUILDINGTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(BUILDING));
	//glm::mat4 BUILDINGNormalMatrix = glm::mat4(1.0f);
	//unsigned int BUILDINGNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	//glUniformMatrix4fv(BUILDINGNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(BUILDINGNormalMatrix));
	//unsigned int BUILDINGfragLocation = glGetUniformLocation(shaderID, "objColor");
	//glUniform3f(BUILDINGfragLocation, 0.6f, 0.6f, 0.5f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());
	//}

	glBindVertexArray(VAO[9]);
	
	unsigned int StageBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(StageBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	glm::mat4 StageTrasMatrix = glm::mat4(1.0f);
	StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_StageX+8.0f, TransList.T_StageY, TransList.T_StageZ+24.0f));
	StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(AngleList.StageAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	StageTrasMatrix = glm::scale(StageTrasMatrix, glm::vec3(24.0,1.0, 6.0));
	unsigned int StageTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(StageTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageTrasMatrix));
	glm::mat4 StageNormalMatrix = glm::mat4(1.0f);
	unsigned int StageNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(StageNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[4].size());

	


	glBindVertexArray(VAO[4]);
	unsigned int Stage2BlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(Stage2BlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	glm::mat4 Stage2TrasMatrix = glm::mat4(1.0f);
	Stage2TrasMatrix = glm::translate(Stage2TrasMatrix, glm::vec3(TransList.T_StageX, TransList.T_StageY , TransList.T_StageZ+10.0f));
	Stage2TrasMatrix = glm::rotate(Stage2TrasMatrix, glm::radians(AngleList.StageAngle2), glm::vec3(0.0f, 1.0f, 0.0f));
	Stage2TrasMatrix = glm::scale(Stage2TrasMatrix, glm::vec3(6.0, 1.0, 30.0f));
	unsigned int Stage2TransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(Stage2TransMatrixLocation, 1, GL_FALSE, glm::value_ptr(Stage2TrasMatrix));
	glm::mat4 Stage2NormalMatrix = glm::mat4(1.0f);
	unsigned int Stage2NormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(Stage2NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(Stage2NormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[9].size());

	


	//·Îº¿ ¸öÅë & ¸Ó¸® 
	glBindVertexArray(VAO[1]);
	unsigned int BODYBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(BODYBlendCheck, 0);
	glm::mat4 BODY = glm::mat4(1.0f);
	BODY = glm::translate(BODY, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy-0.2f, TransList.T_Bodyz));
	BODY = glm::rotate(BODY, glm::radians(AngleList.BodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	BODY = glm::scale(BODY, glm::vec3(0.3f, Scalepos.My+0.4, 0.3f));
	unsigned int BODYLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(BODYLocation, 1, GL_FALSE, glm::value_ptr(BODY));
	glm::mat4 BODYNormal = glm::mat4(1.0f);
	BODYNormal = glm::rotate(BODYNormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int BODYNormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(BODYNormalLocation, 1, GL_FALSE, glm::value_ptr(BODYNormal));
	unsigned int BODYfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(BODYfragLocation, 0.6f, 0.45f, 0.65f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

	//¿ÞÂÊ ÆÈ
	glBindVertexArray(VAO[5]);
	unsigned int ArmLegBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(ArmLegBlendCheck, 0);
	glm::mat4 LEFTARM = glm::mat4(1.0f);
	LEFTARM = glm::translate(LEFTARM, glm::vec3(TransList.T_ArmLegx + 0.2f, TransList.T_ArmLegy - 0.2f, TransList.T_ArmLegz - 2.15f));
	if (Mainswingchk == 1) {
		LEFTARM = glm::rotate(LEFTARM, glm::radians(AngleList.ArmAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		LEFTARM = glm::translate(LEFTARM, glm::vec3(0.18f, 0.0f, 0.15f));
		LEFTARM = glm::rotate(LEFTARM, glm::radians(AngleList.MainSwing1), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (Mainswingchk == 2) {
		LEFTARM = glm::translate(LEFTARM, glm::vec3(-0.18f, 0.1f, -0.2f));
		LEFTARM = glm::rotate(LEFTARM, glm::radians(AngleList.ArmAngle+180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		LEFTARM = glm::rotate(LEFTARM, glm::radians(AngleList.MainSwing1), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	LEFTARM = glm::scale(LEFTARM, glm::vec3(Scalepos.ALx, Scalepos.ALy , Scalepos.ALz));
	unsigned int LEFTARMLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(LEFTARMLocation, 1, GL_FALSE, glm::value_ptr(LEFTARM));
	glm::mat4 LEFTARMNormal = glm::mat4(1.0f);
	LEFTARMNormal = glm::rotate(LEFTARMNormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int LEFTARMNormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(LEFTARMNormalLocation, 1, GL_FALSE, glm::value_ptr(LEFTARMNormal));
	unsigned int LEFTARMfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(LEFTARMfragLocation, 0.847f, 0.45f, 0.628f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[5].size());

	//¿À¸¥ÂÊ ÆÈ
	glBindVertexArray(VAO[6]);
	unsigned int ArmLeg2BlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(ArmLeg2BlendCheck, 0);
	glm::mat4 RIGHTARM = glm::mat4(1.0f);
	RIGHTARM = glm::translate(RIGHTARM, glm::vec3(TransList.T_ArmLegx - 0.2f, TransList.T_ArmLegy-0.2f, TransList.T_ArmLegz - 1.85f));
	if (Mainswingchk == 1) {
		RIGHTARM = glm::rotate(RIGHTARM, glm::radians(AngleList.ArmAngle-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		RIGHTARM = glm::translate(RIGHTARM, glm::vec3(-0.18f, 0.0f, -0.15f));
		RIGHTARM = glm::rotate(RIGHTARM, glm::radians(AngleList.MainSwing2), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (Mainswingchk == 2) {
		RIGHTARM = glm::translate(RIGHTARM, glm::vec3(0.1,0.55,0.2));
		RIGHTARM = glm::rotate(RIGHTARM, glm::radians(AngleList.ArmAngle-110.0f),  glm::vec3(1.0f, 0.0f, 0.0f));
		RIGHTARM = glm::translate(RIGHTARM, glm::vec3(0.18f, -0.2f, -0.35f));
		RIGHTARM = glm::rotate(RIGHTARM, glm::radians(AngleList.MainSwing2), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	RIGHTARM = glm::scale(RIGHTARM, glm::vec3(Scalepos.ALx, Scalepos.ALy , Scalepos.ALz));
	unsigned int RIGHTARMLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(RIGHTARMLocation, 1, GL_FALSE, glm::value_ptr(RIGHTARM));
	glm::mat4 RIGHTARMNormal = glm::mat4(1.0f);
	RIGHTARMNormal = glm::rotate(RIGHTARMNormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int RIGHTARMNormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(RIGHTARMNormalLocation, 1, GL_FALSE, glm::value_ptr(RIGHTARMNormal));
	unsigned int RIGHTARMfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(RIGHTARMfragLocation, 0.847f, 0.45f, 0.628f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[6].size());

	//¿ÞÂÊ ¹ß
	glBindVertexArray(VAO[7]);
	unsigned int ArmLeg3BlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(ArmLeg3BlendCheck, 0);
	glm::mat4 LEFTLEG = glm::mat4(1.0f);
	LEFTLEG = glm::translate(LEFTLEG, glm::vec3(TransList.T_ArmLegx + 0.03f, TransList.T_ArmLegy - 0.5f, TransList.T_ArmLegz - 2.1f));
	if (Mainswingchk == 1) {
		LEFTLEG = glm::rotate(LEFTLEG, glm::radians(AngleList.LegAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		LEFTLEG = glm::translate(LEFTLEG, glm::vec3(0.08f, 0.0f, 0.1f));
		LEFTLEG = glm::rotate(LEFTLEG, glm::radians(AngleList.MainSwing2), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (Mainswingchk == 2) {
		LEFTLEG = glm::rotate(LEFTLEG, glm::radians(AngleList.LegAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		LEFTLEG = glm::translate(LEFTLEG, glm::vec3(0.08f, 0.0f, 0.05f));
		LEFTLEG = glm::rotate(LEFTLEG, glm::radians(AngleList.MainSwing2), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	LEFTLEG = glm::scale(LEFTLEG, glm::vec3(Scalepos.ALx, Scalepos.ALy, Scalepos.ALz));
	unsigned int LEFTLEGLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(LEFTLEGLocation, 1, GL_FALSE, glm::value_ptr(LEFTLEG));
	glm::mat4 LEFTLEGNormal = glm::mat4(1.0f);
	LEFTLEGNormal = glm::rotate(LEFTARMNormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int LEFTLEGNormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(LEFTLEGNormalLocation, 1, GL_FALSE, glm::value_ptr(LEFTLEGNormal));
	unsigned int LEFTLEGfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(LEFTLEGfragLocation, 0.847f, 0.45f, 0.628f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[7].size());

	//¿À¸¥ÂÊ ¹ß
	glBindVertexArray(VAO[8]);
	unsigned int ArmLeg4BlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(ArmLeg4BlendCheck, 0);
	glm::mat4 RIGHTLEG = glm::mat4(1.0f);
	RIGHTLEG = glm::translate(RIGHTLEG, glm::vec3(TransList.T_ArmLegx - 0.03f, TransList.T_ArmLegy - 0.5f, TransList.T_ArmLegz - 1.9f));

	if (Mainswingchk == 1) {
		RIGHTLEG = glm::rotate(RIGHTLEG, glm::radians(AngleList.LegAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		RIGHTLEG = glm::translate(RIGHTLEG, glm::vec3(-0.08f, 0.0f, -0.1f));
		RIGHTLEG = glm::rotate(RIGHTLEG, glm::radians(AngleList.MainSwing1), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (Mainswingchk == 2) {
		RIGHTLEG = glm::rotate(RIGHTLEG, glm::radians(AngleList.LegAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		RIGHTLEG = glm::translate(RIGHTLEG, glm::vec3(-0.08f, 0.0f, -0.05f));
		RIGHTLEG = glm::rotate(RIGHTLEG, glm::radians(AngleList.MainSwing1), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	RIGHTLEG = glm::scale(RIGHTLEG, glm::vec3(Scalepos.ALx, Scalepos.ALy, Scalepos.ALz));
	unsigned int RIGHTLEGLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(RIGHTLEGLocation, 1, GL_FALSE, glm::value_ptr(RIGHTLEG));
	glm::mat4 RIGHTLEGNormal = glm::mat4(1.0f);
	RIGHTLEGNormal = glm::rotate(RIGHTLEGNormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int RIGHTLEGNormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(RIGHTLEGNormalLocation, 1, GL_FALSE, glm::value_ptr(RIGHTLEGNormal));
	unsigned int RIGHTLEGfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(RIGHTLEGfragLocation, 0.847f, 0.45f, 0.628f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[8].size());

	glBindVertexArray(VAO[10]);
	unsigned int EYEBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(EYEBlendCheck, 0);
	glm::mat4 EYE = glm::mat4(1.0f);
	EYE = glm::translate(EYE, glm::vec3(TransList.T_Bodyx - 0.1f, TransList.T_Bodyy, TransList.T_Bodyz + 0.25f));

	if (Mainswingchk == 1) {
		EYE = glm::rotate(EYE, glm::radians(AngleList.BodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (Mainswingchk == 2) {
		EYE = glm::rotate(EYE, glm::radians(AngleList.BodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	EYE = glm::scale(EYE, glm::vec3(0.04f,0.04f,0.04f));
	unsigned int EYELocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(EYELocation, 1, GL_FALSE, glm::value_ptr(EYE));
	glm::mat4 EYENormal = glm::mat4(1.0f);
	BODYNormal = glm::rotate(EYENormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int EYENormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(EYENormalLocation, 1, GL_FALSE, glm::value_ptr(EYENormal));
	unsigned int EYEfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(EYEfragLocation, 0.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[10].size());

	glBindVertexArray(VAO[11]);
	unsigned int EYEBlendCheck2 = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(EYEBlendCheck2, 0);
	glm::mat4 EYE2 = glm::mat4(1.0f);
	EYE2 = glm::translate(EYE2, glm::vec3(TransList.T_Bodyx+0.1f, TransList.T_Bodyy, TransList.T_Bodyz+0.25f));
	if (Mainswingchk == 1) {
		EYE2 = glm::rotate(EYE2, glm::radians(AngleList.BodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (Mainswingchk == 2) {
		EYE2 = glm::rotate(EYE2, glm::radians(AngleList.BodyAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	EYE2 = glm::scale(EYE2, glm::vec3(0.04f, 0.04f, 0.04f));
	unsigned int EYELocation2 = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(EYELocation2, 1, GL_FALSE, glm::value_ptr(EYE2));
	glm::mat4 EYENormal2 = glm::mat4(1.0f);
	EYENormal2 = glm::rotate(EYENormal2, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int EYENormalLocation2 = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(EYENormalLocation2, 1, GL_FALSE, glm::value_ptr(EYENormal2));
	unsigned int EYEfragLocation2 = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(EYEfragLocation2, 0.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[11].size());

	//--------------TRAP
	glBindVertexArray(VAO[12]);
	unsigned int RECOILBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(RECOILBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glm::mat4 RECOIL = glm::mat4(1.0f);
	RECOIL = glm::translate(RECOIL, glm::vec3(TransList.T_Trapx, TransList.T_Trapy, TransList.T_Trapz+16.0));
	RECOIL = glm::rotate(RECOIL, glm::radians(AngleList.AngleTrap), glm::vec3(1.0f, 0.0f, 0.0f));
	RECOIL = glm::scale(RECOIL, glm::vec3(0.6f,0.6f,0.6f));
	unsigned int RECOILLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(RECOILLocation, 1, GL_FALSE, glm::value_ptr(RECOIL));
	glm::mat4 RECOILNormal = glm::mat4(1.0f);
	RECOILNormal = glm::rotate(RECOILNormal, glm::radians(AngleList.angle), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int RECOILNormalLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(RECOILNormalLocation, 1, GL_FALSE, glm::value_ptr(RECOILNormal));
	unsigned int RECOILfragLocation = glGetUniformLocation(shaderID, "objColor");
	glUniform3f(RECOILfragLocation, 0.6f, 0.45f, 0.65f);
	glDrawArrays(GL_TRIANGLES, 0, Vertex[12].size());
	if ((TransList.T_Trapz+16.0f)-0.6f <= TransList.T_Bodyz&& (TransList.T_Trapz + 16.0f) +0.6f >= TransList.T_Bodyz &&TransList.T_Trapx+ 0.6 >= TransList.T_Bodyx&& TransList.T_Trapx -0.6 <= TransList.T_Bodyx)
	{
		TransList.T_Bodyz-= 0.2f;
		TransList.T_ArmLegz-= 0.2f;
	}

}
void initTexture()
{

	glGenTextures(2, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int widthImage, heightImage, numberOfChannel;
	unsigned char* data = stbi_load("bd.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int PYwidthImage, PYheightImage, PYnumberOfChannel;
	unsigned char* PYdata = stbi_load("123.jpg", &PYwidthImage, &PYheightImage, &PYnumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PYwidthImage, PYheightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, PYdata);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(PYdata);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int BackwidthImage, BackheightImage, BacknumberOfChannel;
	unsigned char* Backdata = stbi_load("tail.jpg", &BackwidthImage, &BackheightImage, &BacknumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BackwidthImage, BackheightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, Backdata);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(Backdata);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int TailWidthImage, TailHeightImage, TailnumberOfChannel;
	unsigned char* TailData = stbi_load("tail.jpg", &TailWidthImage, &TailHeightImage, &TailnumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TailWidthImage, TailHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, TailData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(TailData);

}

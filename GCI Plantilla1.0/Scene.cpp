#include "Scene.h"

using namespace std;

float altu = 7;
bool start = true;
float xd = 0, yd = 0, zd = 0;
float xDroneM = 0, yDroneM = 0, zDroneM = 0;
bool WaterMovmentX = true;
bool WaterMovmentZ = true;
float WaterX = 255;
float WaterZ = 103;

Scene::Scene(OpenGLClass* OpenGLObject, HWND hwnd) {
	handlerWindow = hwnd;
	OpenGL = OpenGLObject;
	Camera = 0;
	LightShader = 0;
	ColorShader = 0;
	ShaderBounding = 0;
	Triangulo = 0;
	Light = 0;
	speedAxisX = 0.0f;
	speedAxisY = 0.0f;
	speed = 0.0f;
	angulo = 0.0f;
	Drone = 0;
	LoaderTexture = new TextureClass(OpenGL);
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	bool result = true;
	string resultDetail = "";

	angulo = 0.0f;
	speedAxisX = SPEED_AXIS_X;
	speedAxisY = SPEED_AXIS_Y;
	speed = SPEED_CAMERA;

	// Crea un objeto camara.
	Camera = new CameraClass;
	if (!Camera) {
		result = false;
		_RPT1(0, "Alert! Camera has not been initialized. \n", 0);
		return result;
	}
	else {
		// Asigna las coordenadas de posicion y rotacion por defecto a los arreglos delta.
		DeltaPosition->X = InitialPosition[0]; DeltaPosition->Y = InitialPosition[1]; DeltaPosition->Z = InitialPosition[2];
		DeltaRotation->X = InitialRotation[0]; DeltaRotation->Y = InitialRotation[1]; DeltaRotation->Z = InitialRotation[2];
		// Luego se los pasamos a la camara.
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
		// Con esto podriamos aislar siempre el punto de partida inicial de la escena para evitar teletrasportarse sin querer.
	}

	LightShader = new LightShaderClass(OpenGL, handlerWindow, "shaders/terrain.vs", "shaders/terrain.ps");
	if (!LightShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ColorShader = new ColorShaderClass(OpenGL, handlerWindow, "shaders/color.vs", "shaders/color.ps");
	if (!ColorShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the color shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! ColorShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	//Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/heightmap.jpg", "recursos/test.tga", "recursos/Piedras_normal.jpg",(float)400, (float)400, 0, 1);
	Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"textures/heightmap.jpg", "recursos/Piedras.jpg", "textures/terrain1.jpg", "textures/heightmap.jpg", (float)400, (float)400);
	if (!Terreno) {
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		Terreno->SetLandShader(LightShader);
	}




	Triangulo = new TriangleClass(OpenGL);
	if (!Triangulo) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the triangle model object.", "Error", MB_OK);
		_RPT1(0, "Alert! Triangle has not been initialized. \n", 0);
		return result;
	}
	else {
		Triangulo->SetShaderTriangle(ColorShader);
	}

	ShaderModel = new ShaderTexture(OpenGL, handlerWindow, "shaders/light.vs", "shaders/light.ps");
	if (!ShaderModel) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ShaderBounding = new ShaderBoundingBox(OpenGL, handlerWindow, "shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	if (!ShaderBounding) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Drone = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/drone/drone.obj",
		"recursos/model/drone/drone.jpg");
	if (!Drone) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Drone->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Drone->SetShaders(ShaderModel, ShaderBounding);
	}

	Water = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/water/water.obj",
		"recursos/model/water/water.jpg");
	if (!Water) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Water->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Water->SetShaders(ShaderModel, ShaderBounding);
		Water->AddTexture("recursos/Official Models/Tree1T2.jpg");
	}

	WoodHouse = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/woodhouse/woodhouse.obj",
		"recursos/model/woodhouse/woodhouse.png");
	if (!WoodHouse) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = WoodHouse->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		WoodHouse->SetShaders(ShaderModel, ShaderBounding);
	}

	CasaCompleta = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/CasaCompleta/CasaCompleta.obj",
		"recursos/model/CasaCompleta/CasaCompleta.jpg");
	if (!CasaCompleta) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = CasaCompleta->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		CasaCompleta->SetShaders(ShaderModel, ShaderBounding);
	}

	RunasRoja = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/RunasRoja/RunasRoja.obj",
		"recursos/model/RunasRoja/RunasRoja.png");
	if (!RunasRoja) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = RunasRoja->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		RunasRoja->SetShaders(ShaderModel, ShaderBounding);

	}

	table = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/table/table.obj",
		"recursos/model/table/table.jpg");
	if (!table) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = table->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		table->SetShaders(ShaderModel, ShaderBounding);

	}

	LibroMagico = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/LibroMagico/LibroMagico.obj",
		"recursos/model/LibroMagico/LibroMagico.png");
	if (!LibroMagico) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = LibroMagico->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		LibroMagico->SetShaders(ShaderModel, ShaderBounding);

	}

	portal = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/portal/portal.obj",
		"recursos/model/portal/portal.png");
	if (!portal) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = portal->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		portal->SetShaders(ShaderModel, ShaderBounding);

	}

	lamp = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/model/lamp/lamp.obj",
		"recursos/model/lamp/lamp.png");
	if (!lamp) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = lamp->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		lamp->SetShaders(ShaderModel, ShaderBounding);

	}


	// Skydome
	ShaderSky = new SkydomeShaderClass(OpenGL, handlerWindow, "shaders/SkydomeShader.vs", "shaders/SkydomeShader.ps");
	if (!ShaderSky) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Skydome = new Dome("recursos/sky.png", OpenGL, LoaderTexture, 500);
	if (!Skydome) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Skydome->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Skydome->SetIterpolationDay(false, true);
		Skydome->SetShader(ShaderSky);
	}

	// Collision Boxes
	box = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 10.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ -10.0f, -10.0f, -10.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box->SetShader(ShaderBounding);
	}

	box2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box2->SetShader(ShaderBounding);
	}
	box3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box3->SetShader(ShaderBounding);
	}
	box4 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box4->SetShader(ShaderBounding);
	}

	// Billboards
	ShaderBill = new BillboardShaderClass(OpenGL, handlerWindow, "shaders/billboard.vs", "shaders/billboard.ps");
	if (!ShaderBill) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	forest = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/forest.png");
	if (!forest) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		forest->Initialize(15.0f);
		forest->SetShader(ShaderBill);
	}

	glow = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/red.png");
	if (!glow) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		glow->Initialize(2.0f);
		glow->SetShader(ShaderBill);
	}

	magic = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/magic.png");
	if (!magic) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		magic->Initialize(2.0f);
		magic->SetShader(ShaderBill);
	}

	fire = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/fire.png");
	if (!fire) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		fire->Initialize(7.0f);
		fire->SetShader(ShaderBill);
	}

	smoke = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/smoke.png");
	if (!smoke) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		smoke->Initialize(10.0f);
		smoke->SetShader(ShaderBill);
	}

	if (start)
	{
		MessageBoxA(NULL, "Welcome to your drone, setting up fligth protocol Please get to the correct height to start free flying",
			"DroneSoftware 3.0", MB_OK);
		start = false;
	}

	return result;
}

bool Scene::Render() {
	bool result = true;

	// Limpia los buffers para iniciar la escena nueva.
	OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Definimos nuestro espacio para las 3 matrices necesarias para renderizar.
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];



	// Actualiza la posicion y rotacion de la camara
	Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
	Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);

	// Genera la matriz de vista a partir de la posicion de la camara.
	Camera->Render();

	// Obtiene las matrices de vista, mundo y proyeccion de opengl y el objeto de la camara.
	OpenGL->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	OpenGL->GetProjectionMatrix(projectionMatrix);

	// Renderizamos el Skydome
	Skydome->CicloInterpolaciones();
	Skydome->Render(viewMatrix, projectionMatrix);

	// Renderizamos terreno
	Terreno->Render(OpenGL, worldMatrix, viewMatrix, projectionMatrix);

	


	// Renderizamos Triangulo (ya se logra ver en escena)
	Triangulo->Render(viewMatrix, projectionMatrix);

	// Renderizamos nuestros objetos en la escena
	Drone->Render(viewMatrix, projectionMatrix, false);
	Water->Render(viewMatrix, projectionMatrix, false);
	WoodHouse->Render(viewMatrix, projectionMatrix, false);
	CasaCompleta->Render(viewMatrix, projectionMatrix, false);
	RunasRoja->Render(viewMatrix, projectionMatrix, false);
	table->Render(viewMatrix, projectionMatrix, false);
	LibroMagico->Render(viewMatrix, projectionMatrix, false);
	portal->Render(viewMatrix, projectionMatrix, false);
	lamp->Render(viewMatrix, projectionMatrix, false);



	// Renderizamos las cajas de colisión
	/*box->Draw(viewMatrix, projectionMatrix);
	box2->Draw(viewMatrix, projectionMatrix);
	box3->Draw(viewMatrix, projectionMatrix);
	box4->Draw(viewMatrix, projectionMatrix);*/

	// Renderizamos los billboards
	forest->Render(viewMatrix, projectionMatrix,
		0.0f, Terreno->Superficie(0.0f, 0.0f), 100.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	glow->Render(viewMatrix, projectionMatrix, 
		100.0f, Terreno->Superficie(-10.0f, -10.0f),-11.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	magic->Render(viewMatrix, projectionMatrix, 
		-70.0f, Terreno->Superficie(10.0f, 10.0f), 0.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	fire->Render(viewMatrix, projectionMatrix,
		-67.0f, Terreno->Superficie(-10.0f, 10.0f), -29.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	fire->Render(viewMatrix, projectionMatrix,
		-69.0f, Terreno->Superficie(-10.0f, 10.0f), -29.0f,
		DeltaPosition->X, DeltaPosition->Z);

	fire->Render(viewMatrix, projectionMatrix,
		-68.0f, Terreno->Superficie(-10.0f, 10.0f), -31.0f,
		DeltaPosition->X, DeltaPosition->Z);

	smoke->Render(viewMatrix, projectionMatrix, 
		-68.0f, 6.0f, -29.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	// Damos la instruccion de que termino la escena para que nos muestre frame a frame.
	OpenGL->EndScene();

	return result;
}

bool Scene::Update(InputClass* input, float deltaTime) {
	bool result = true;

	this->input = input;
	this->deltaTime = deltaTime;
	result = ManageCommands();
	if (!result) {
		result = false;
		_RPT1(0, "Alert! Error on key. \n", 0);
		return result;
	}

	//agua 

	if (WaterMovmentX == true) {
		WaterX += 1;
		if (WaterX >= 100) {
			WaterMovmentX = false;
		}
	}
	if (WaterMovmentX == false) {
		WaterX = -1;
		if (WaterX <= -100) {
			WaterMovmentX = true;

		}
	}
	if (WaterMovmentZ == true) {
		WaterZ += 1;
		if (WaterZ >= 100) {
			WaterMovmentZ = false;
		}
	}
	if (WaterMovmentZ == false) {
		WaterZ -= 1;
		if (WaterZ <= -100) {
			WaterMovmentZ = true;

		}
	}

	float* matrixWater = Water->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixWater, WaterX, 5.0f, WaterZ);
	//OpenGL->MatrixScale(matrixWater, 10000.0f, 10000.0f, 10000.0f);

	
	
	
	float* matrixWoodHouse = WoodHouse->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixWoodHouse, -110.0f, 5.5f, -10.0f);

	float* matrixCasaCompleta = CasaCompleta->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCasaCompleta, 100.0f, 6.0f, -10.0f);

	float* matrixRunasRoja = RunasRoja->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixRunasRoja, 100.0f, 6.0f, -10.0f);

	float* matrixtable = table->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixtable, -70.0f, 6.0f, 0.0f);

	float* matrixLibroMagico = LibroMagico->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixLibroMagico, -70.0f, 8.58f, 00.0f);

	float* matrixportal = portal->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixportal, 100.0f, 0.0f, -120.0f);

	float* matrixlamp = lamp->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixlamp, 5.0f, 5.9f, -42.0f);

	
	//Tranformaciones de cajas de colisión
	float* auxMatrix = new float[16]{ 0.0f };
	OpenGL->BuildIdentityMatrix(auxMatrix);

	float* matrixBox = box->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox, 0.0f, 10.0f, 90.0f);

	OpenGL->MatrixScale(auxMatrix, 20.0f, 5.0f, 1.0f);
	OpenGL->MatrixMultiply(matrixBox, matrixBox, auxMatrix);

	float* matrixBox2 = box2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox2, -20.0f, 10.0f, -90.0f);

	OpenGL->MatrixScale(auxMatrix, 20.0f, 5.0f, 1.0f);
	OpenGL->MatrixMultiply(matrixBox2, matrixBox2, auxMatrix);

	float* matrixBox3 = box3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox3, -210.0f, 10.0f, 1.0f);

	OpenGL->MatrixScale(auxMatrix, 1.0f, 5.0f, 10.0f);
	OpenGL->MatrixMultiply(matrixBox3, matrixBox3, auxMatrix);

	float* matrixBox4 = box4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox4, 160.0f, 10.0f, 0.0f);

	OpenGL->MatrixScale(auxMatrix, 1.0f, 5.0f, 10.0f);
	OpenGL->MatrixMultiply(matrixBox4, matrixBox4, auxMatrix);

	//Colisión por esfera
	/*if (Drone->GetSphericalCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z, 2)) {
		MessageBox(handlerWindow, L"Colisionando", L"Aviso", MB_OK);
	}*/

	//Colisión por caja
	if (Drone->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (WoodHouse->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (CasaCompleta->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Warning collision imminent, taking control",
			"Collision detection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		altu = 30;
	}

	if (RunasRoja->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Warning collision imminent, taking control",
			"Collision detection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		
	}

	if (table->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Warning collision imminent, taking control",
			"Collision detection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;

	}

	if (LibroMagico->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Warning collision imminent, taking control",
			"Collision detection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;

	}

	if (portal->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Warning collision imminent, taking control",
			"Collision detection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;

	}

	if (lamp->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Warning collision imminent, taking control",
			"Collision detection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;

	}

	if (box->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Weak signal, please get closer to the controller",
			"Signal protection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (box2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Weak signal, please get closer to the controller",
			"Signal protection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (box3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Weak signal, please get closer to the controller",
			"Signal protection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (box4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		MessageBoxA(NULL, "Weak signal, please get closer to the controller",
			"Signal protection system", MB_OK);
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}



	return result;
}

bool Scene::ManageCommands()
{
	bool result = true;

	
	xd = DeltaPosition->X;
	yd = DeltaPosition->Y;
	zd = DeltaPosition->Z;

	xDroneM = xd;
	yDroneM = yd;
	zDroneM = zd;


	float* matrixDrone = Drone->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixDrone, xDroneM, yDroneM+0.08f, zDroneM);


	//Guardar la posición de la Cámara antes de actualizarla
	LastDeltaPosition->X = DeltaPosition->X;
	LastDeltaPosition->Y = DeltaPosition->Y;
	LastDeltaPosition->Z = DeltaPosition->Z;

	float vr[2]{ 0,0 };
	//float altu = 10;
	float radians = DeltaRotation->Y * 0.0174532925f;

	if (input->GetKey(KeyCode.P)) {
		// Create a std::string with the information you want to display
		std::string info = "X: " + std::to_string(DeltaPosition->X)
			+ " Y: " + std::to_string(DeltaPosition->Y)
			+ " Z: " + std::to_string(DeltaPosition->Z);

		// Calculate the required buffer size for the wide character string
		int bufferLength = MultiByteToWideChar(CP_UTF8, 0, info.c_str(), -1, NULL, 0);

		// Allocate a buffer for the wide character string
		wchar_t* wideInfo = new wchar_t[bufferLength];

		// Convert the std::string to a wide character string
		MultiByteToWideChar(CP_UTF8, 0, info.c_str(), -1, wideInfo, bufferLength);

		// Now, you have 'wideInfo' as a wide character string, which can be displayed correctly in MessageBox.
		MessageBox(NULL, wideInfo, L"Porque esta en chino", MB_OK);

		// Don't forget to release the allocated memory
		delete[] wideInfo;
	}


	if ((input->GetKey(KeyCode.W) || input->GetKey(KeyCode.Up))) {
		
		zDroneM = zDroneM - 5;

		if(altu<18)
		{
			MessageBoxA(NULL, "Please get to the correct height to start free flying",
				"Launch system", MB_OK);
		}
		else
		{
		vr[0] += sinf(radians) * speed * deltaTime;
		vr[1] += cosf(radians) * speed * deltaTime;
		}
	}
	if ((input->GetKey(KeyCode.S) || input->GetKey(KeyCode.Down))) {
		
		
		
		if (altu < 18)
		{
			MessageBoxA(NULL, "Please get to the correct height to start free flying",
				"Launch system", MB_OK);
		}
		else
		{
		vr[0] -= sinf(radians) * speed * deltaTime;
		vr[1] -= cosf(radians) * speed * deltaTime;
		}
	//test
	}
	if ((input->GetKey(KeyCode.A) || input->GetKey(KeyCode.Left))) {
		
		xDroneM = xDroneM + 5;

		if (altu < 18)
		{
			xDroneM = xDroneM + 5;
			MessageBoxA(NULL, "Please get to the correct height to start free flying",
				"Launch system", MB_OK);
		}
		else
		{
		vr[0] -= sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		vr[1] -= cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		}
	}
	if ((input->GetKey(KeyCode.D) || input->GetKey(KeyCode.Right))) {



		xDroneM = xDroneM - 5;
		if (altu < 18)
		{
			MessageBoxA(NULL, "Please get to the correct height to start free flying",
				"Launch system", MB_OK);
		}
		else{
		vr[0] += sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		vr[1] += cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		}
	}



	
	//derecha e izq es  x
	//adelante y atraz es z
	if (input->GetKey(KeyCode.Space))
	{
		//max Y 60
		if (altu >= 60)
		{
			altu--;
			MessageBox(NULL, L"Max flying height", L"Fligth protection system", MB_OK | MB_ICONWARNING);
		}
		else
		{
			altu++;
		}

	}

	if (input->GetKey(KeyCode.Control))
	{

		//max Y 60
		if (altu <= 19)
		{
			altu++;
			MessageBox(NULL, L"Minimum flying height", L"Fligth protection system", MB_OK | MB_ICONWARNING);
		}
		else
		{
			altu--;
		}
	}

		//minimo Y 19 


		if (input->GetRightAxisY() || input->GetRightAxisX()) {
			float angleX = input->GetRightAxisY() * speedAxisX * deltaTime;
			float angleY = input->GetRightAxisX() * speedAxisY * deltaTime;
			DeltaRotation->X -= angleX;
			DeltaRotation->Y += angleY;
		}



		if (input->GetKey(KeyCode.I)) {
			DeltaRotation->X -= speedAxisX * deltaTime;
		}
		if (input->GetKey(KeyCode.K)) {
			DeltaRotation->X += speedAxisX * deltaTime;
		}
		if (input->GetKey(KeyCode.J)) {
			DeltaRotation->Y -= speedAxisY * deltaTime;
		}
		if (input->GetKey(KeyCode.L)) {
			DeltaRotation->Y += speedAxisY * deltaTime;
		}
		if (input->GetKey(KeyCode.One)) {
			Water->ChangeTexture(0, 0);
		}
		if (input->GetKey(KeyCode.Two)) {
			Water->ChangeTexture(1, 1);
		}
		if (input->GetKey(KeyCode.Three)) {
			Water->ChangeTexture(2, 2);
		}
		if (input->GetKey(KeyCode.Four)) {
			Water->ChangeTexture(3, 3);
		}
		if (input->GetKey(KeyCode.Five)) {
			Water->ChangeTexture(4, 4);
		}
		if (input->GetKey(KeyCode.Six)) {
			Water->ChangeTexture(5, 5);
		}
		if (input->GetKey(KeyCode.Seven)) {
			Water->ChangeTexture(6, 6);
		}
		if (input->GetKey(KeyCode.Eight)) {
			Water->ChangeTexture(7, 7);
		}
		if (input->GetKey(KeyCode.Nine)) {
			Water->ChangeTexture(8, 8);
		}
		if (input->GetKey(KeyCode.Zero)) {
			Water->ChangeTexture(9, 9);
		}


		//DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;

		DeltaPosition->Y = altu;

		DeltaPosition->X += vr[0];
		DeltaPosition->Z += vr[1];

		return result;
	
	}




bool Scene::Shutdown() 
{
	bool result = true;

	if (Camera)
	{
		delete Camera;
		Camera = 0;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
		LightShader = 0;
	}

	if (Triangulo) {
		Triangulo->Shutdown();
		delete Triangulo;
		Triangulo = 0;
	}

	if (Drone) {
		Drone->Shutdown();
		delete Drone;
		Drone = 0;
	}

	return result;
}
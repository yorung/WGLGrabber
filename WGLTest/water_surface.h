struct WaterVert
{
	Vec3 pos;
	Vec3 normal;
};

struct WaterRipple
{
	WaterRipple()
	{
		generatedTime = -10000;
	}
	double generatedTime;
	Vec2 centerPos;
};

class VertexObjs
{
public:
	VertexObjs()
	{
		vbo = ibo = 0;
	}
	void Destory()
	{
		afSafeDeleteBuffer(vbo);
		afSafeDeleteBuffer(ibo);
	}
	GLuint vbo, ibo;
};

class WaterSurface
{
	ShaderMan::SMID shaderId;
	ShaderMan::SMID shaderIdFullScr;
	int lines;
	void UpdateVert(std::vector<WaterVert>& vert);
	void Update();
	WaterRipple ripples[2];
	int ripplesNext;
	double elapsedTime;
	double lastTime;
	double nextTime;
	VertexObjs surfaceVtxObjs;
	VertexObjs fullScrVtxObjs;
	int nIndi;
	GLuint samplerClamp;
	GLuint samplerRepeat;
	GLuint samplerNoMipmap;
	GLuint texRenderTarget;
	GLuint framebufferObject;
	GLuint renderbufferObject;
public:
	WaterSurface();
	~WaterSurface();
	void Destroy();
	void Init();
	void Update(int w, int h);
	void Draw();
	void CreateRipple(Vec2 pos);
};

extern WaterSurface waterSurface;

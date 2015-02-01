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
		vbo = ibo = vao = 0;
	}
	void Destory()
	{
		afSafeDeleteBuffer(vbo);
		afSafeDeleteBuffer(ibo);
		if (vao) {
			glDeleteVertexArrays(1, &vao);
			vao = 0;
		}
	}
	GLuint vbo, ibo, vao;
};

class WaterSurface
{
	ShaderMan::SMID shaderId;
	ShaderMan::SMID shaderIdFullScr;
	int lines;
	void UpdateVert(std::vector<WaterVert>& vert);
	void UpdateBuffers();
	WaterRipple ripples[2];
	int ripplesNext;
	double elapsedTime;
	double lastTime;
	double nextTime;
	VertexObjs surfaceVtxObjs;
	VertexObjs fullScrVtxObjs;
	GLuint ubo;
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

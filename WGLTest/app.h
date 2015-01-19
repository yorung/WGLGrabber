class App {
public:
	void Create();
	void Update(float aspect, float offset);
	void Draw();
	void CreateRipple(float x, float y);
	void Destroy();
};

extern App app;

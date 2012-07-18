#include <vector>

// fwd decl
struct Light_t;

class Material;

class Object;
typedef std::vector<Object*> SceneList;

class Scene
{
public:
	void createTestScene();

	std::vector<Light_t*> lights;
	std::vector<Object*> primitives;
	std::vector<Material*> materials;
};

class ChunkGenerator {

public:
	int chunkHeight;
	int chunkWidth;
	int visibleChunksX;
	int visibleChunksY;
	float* vertices;

	ChunkGenerator(int height, int width, int seenX, int seenY);




private:



};


ChunkGenerator::ChunkGenerator(int height, int width, int seenX, int seenY) {
	chunkHeight = height;
	chunkWidth = width;
	visibleChunksX = seenX;
	visibleChunksY = seenY;
	vertices = new float[chunkHeight * chunkWidth * visibleChunksX * visibleChunksY * 6 * 3];
}
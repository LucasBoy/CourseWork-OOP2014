/// <summary>
/// Holds matrix coordinates
/// </summary>
#ifndef _COORDINATES
#define _COORDINATES
#include <fstream>
class Coordinates {
public:
	Coordinates();
	Coordinates(size_t, size_t);
	Coordinates(const Coordinates&);
	Coordinates& operator=(const Coordinates&);
	Coordinates(std::ifstream& file);

	void Save(std::ofstream& file) const;

	// Getters and setters
	size_t GetX() const { return x; }
	size_t GetY() const { return y; }
	void SetX(const size_t x) {	this->x = x; }
	void SetY(const size_t y) {	this->y = y; }

	// Additional operators
	bool operator==(const Coordinates&);
	bool operator!=(const Coordinates&);

private:
	size_t x;
	size_t y;
};
#endif

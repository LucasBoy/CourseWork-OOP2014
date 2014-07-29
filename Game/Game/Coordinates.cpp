#include "Coordinates.h"

Coordinates::Coordinates() {
	this->SetX(0);
	this->SetY(0);
}

Coordinates::Coordinates(size_t x, size_t y) {
	this->SetX(x);
	this->SetY(y);
}

Coordinates::Coordinates(const Coordinates& other) {
	*this = other;
}

Coordinates& Coordinates::operator=(const Coordinates& other) {
	if (this != &other) {
		this->SetX(other.GetX());
		this->SetY(other.GetY());
	}

	return *this;
}

Coordinates::Coordinates(std::ifstream& file) {
	size_t x;
	size_t y;
	file.read((char*)&x, sizeof(x));
	file.read((char*)&y, sizeof(y));

	if (!file.good()) {
		throw "Coordinates cannot be loaded!";
	}
	
	SetX(x);
	SetY(y);
}

bool Coordinates::operator==(const Coordinates& other) {
	if (this->GetX() == other.GetX() && this->GetY() == other.GetY()) {
		return true;
	}
	return false;
}

bool Coordinates::operator!=(const Coordinates& other) {
	return !this->operator==(other);
}

void Coordinates::Save(std::ofstream& file) const {
	file.write((char*)&x, sizeof(x));
	file.write((char*)&y, sizeof(y));

	if (!file.good()) {
		throw "Coordinates not saved!";
	}
}
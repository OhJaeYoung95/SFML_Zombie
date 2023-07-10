#include "stdafx.h"
#include "Utils.h"


std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::RandomRange(int min, int maxExclude)
{
	std::uniform_int_distribution<> dist(min, maxExclude - 1);
	return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(gen);
}

sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f result(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
	result = Normalize(result) * radius;
	return result;
}

sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}

void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}

void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}

void Utils::SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	obj.setOrigin(originPos);
}

void Utils::SetOrigin(sf::VertexArray& vertex, Origins origin)
{
	// ¾È‰Î
	sf::Vector2f originPos(vertex.getBounds().width, vertex.getBounds().height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position -= originPos;
	}
}

float Utils::Clamp(float v, float min, float max)
{
	return std::max(min, std::min(max, v));
	//if (v < min)
	//	return min;
	//if (v > max)
	//	return max;
	//return v;
}

sf::Vector2f& Utils::Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max)
{
	sf::Vector2f result;
	result.x = Clamp(v.x, min.x, max.x);
	result.y = Clamp(v.y, min.y, max.y);
	return result;
}

float Utils::Lerp(float a, float b, float t, bool clamping)
{
	float v = a + (b - a) * t;
	if (clamping)
	{
		v = Utils::Clamp(v, std::min(a, b), std::max(a, b));
	}
	return v;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping)
{
	if (clamping)
	{
		t = Utils::Clamp(t, 0.f, 1.f);
	}
	return a + Utils::Normalize(b - a) * (Utils::Distance(a, b) * t);
}

sf::Color Utils::Lerp(const sf::Color& a, const sf::Color& b, float t, bool clamping)
{
	sf::Color v;
	v.r = Lerp(a.r, b.r, t, clamping);
	v.g = Lerp(a.g, b.g, t, clamping);
	v.b = Lerp(a.b, b.b, t, clamping);
	v.a = Lerp(a.a, b.a, t, clamping);

	return v;
}


const sf::Vector2f Utils::Normalize(const sf::Vector2f& vector)
{
	float mag = Magnitude(vector);
	if (mag == 0.f)
	{
		return vector;
	}
	return vector / mag;
}

float Utils::Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
	return Magnitude(pos2 - pos1);
}

float Utils::Magnitude(const sf::Vector2f& vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Utils::SqrMagnitude(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

float Utils::Angle(const sf::Vector2f& start, const sf::Vector2f& end)
{
	return Angle(Normalize(end - start));
}

float Utils::Angle(const sf::Vector2f& dir)
{
	return atan2(dir.y, dir.x) * (180.f / M_PI);
}

sf::Vector2f Utils::DirectionFromAngle(float angle)
{
	float radians = angle * (M_PI / 180.f);
	return sf::Vector2f(std::cos(radians), std::sin(radians));
}

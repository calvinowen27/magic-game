#ifndef VECTOR2_INCLUDE
#define VECTOR2_INCLUDE

#include <iostream>
#include <string>

class Vector2Int;

class Vector2
{
    public:
        float x, y;
        static Vector2 zero;

        /// @brief default constructor, initializes x and y to 0
        Vector2();

        Vector2(float x, float y); 

        /// @brief make a copy of [b]
        /// @param b Vector2 to make a copy of
        Vector2(const Vector2& b);

        /// @param a 
        /// @param b 
        /// @return pythagorean distance between [a] and [b], which can be treated as points
        static float distance(const Vector2& a, const Vector2& b);

        /// @brief checks for equality between [this] and [b]
        /// @param b 
        /// @return boolean value indicating equality
        bool equals(const Vector2& b) const;

        /// @brief normalizes [this]
        void normalize();

        /// @return copy of [this], normalized
        Vector2 normalized() const;

        /// @return the magnitude/length of [this]
        float magnitude() const;

        /// @brief  get a Vector2 with both x = y = +-1, with the sign being the corresponding signs of [this] 
        /// @return a Vector2 indicating the direction of [this]
        Vector2 getDir() const;
        
        Vector2 abs() const; // returns a vector with x = abs(x) and y = abs(y)
        Vector2 round(int decimals) const; // rounds the values of this vector to [decimals] decimal places
        friend bool operator==(const Vector2& a, const Vector2& b);
        friend bool operator!=(const Vector2& a, const Vector2& b);
        Vector2 operator+(const Vector2& b) const; // returns a new vector where x = [this].x + [b].x and y = [this].y + [b].y
        Vector2 operator-(const Vector2& b) const; // returns a new vector where x = [this].x - [b].x and y = [this].y - [b].y
        Vector2 operator*(const float& a) const; // scalar multiplication [a]*[this]
        Vector2 operator*(const Vector2& b) const; // cross product between [this] and [b]
        Vector2 operator/(const float& a) const; // scalar division [this]/[a]
        Vector2 operator/(const Vector2& b) const; // cross product with [this] and the inverse of [b] or [b]^-1
        Vector2& operator=(const Vector2& b);
        Vector2& operator+=(const Vector2& b);
        Vector2& operator-=(const Vector2& b);
        Vector2& operator*=(const float& a);
        Vector2& operator/=(const float& a);
        std::string to_string() const; // returns string representation of [this] in the form "(x, y)"
        friend std::ostream& operator<<(std::ostream& stream, const Vector2& v); // insert string representation of [v] into [stream]
        explicit operator Vector2Int() const; // cast [this] as a Vector2Int where x = (int)[this].x and y = (int)[this].y
};

class Vector2Int
{
    public:
        int x, y;
        Vector2Int();
        Vector2Int(int x, int y);
        Vector2Int(float x, float y);
        Vector2Int(double x, double y);
        Vector2Int(const Vector2Int& b);
        friend float distance(Vector2Int a, Vector2Int b);
        bool equals(const Vector2Int& b) const;
        void normalize();
        friend bool operator==(const Vector2Int& a, const Vector2Int& b);
        Vector2Int operator+(const Vector2Int& b) const;
        Vector2Int operator-(const Vector2Int& b) const;
        Vector2Int operator*(const float& a) const;
        Vector2Int operator/(const float& a) const;
        Vector2Int& operator=(const Vector2Int& b);
        Vector2Int& operator+=(const Vector2Int& b);
        Vector2Int& operator-=(const Vector2Int& b);
        Vector2Int& operator*=(const float& a);
        Vector2Int& operator/=(const float& a);
        std::string to_string() const;
        friend std::ostream& operator<<(std::ostream& stream, const Vector2Int& v);
        explicit operator Vector2() const;
};

#endif
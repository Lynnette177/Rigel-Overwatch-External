#pragma once
using namespace DirectX;
namespace OW {
	class Rect {
	public:
		float x;
		float y;
		float width;
		float height;

		Rect() {
			this->x = 0;
			this->y = 0;
			this->width = 0;
			this->height = 0;
		}

		Rect(float x, float y, float width, float height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		bool operator==(const Rect& src) const {
			return (src.x == this->x && src.y == this->y && src.height == this->height && src.width == this->width);
		}

		bool operator!=(const Rect& src) const {
			return (src.x != this->x && src.y != this->y && src.height != this->height && src.width != this->width);
		}
	};
	class Vector2 {
	public:
		float X, Y;

		Vector2() { X = 0; Y = 0; }

		Vector2(float _X, float _Y)
		{
			X = _X;
			Y = _Y;
		}

		inline float Distance(Vector2 v)
		{
			return float(sqrtf(powf(v.X - X, 2.0) + powf(v.Y - Y, 2.0)));
		}

		inline float get_length()
		{
			return float(sqrtf(powf(X, 2.0) + powf(Y, 2.0)));
		}

		inline Vector2 operator+(Vector2 v)
		{
			return { X + v.X, Y + v.Y };
		}

		inline Vector2 operator-(Vector2 v)
		{
			return { X - v.X, Y - v.Y };
		}
	};
#pragma pack(push, 1)
	struct Vector3
	{
	public:
		float X;
		float Y;
		float Z;

		inline Vector3()
			: X(0), Y(0), Z(0) {
		}

		inline Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

		__forceinline Vector3 operator-(const Vector3& V) {
			return Vector3(X - V.X, Y - V.Y, Z - V.Z);
		}

		__forceinline Vector3 operator+(const Vector3& V) {
			return Vector3(X + V.X, Y + V.Y, Z + V.Z);
		}

		__forceinline Vector3 operator*(float Scale) const {
			return Vector3(X * Scale, Y * Scale, Z * Scale);
		}

		__forceinline Vector3 operator/(float Scale) const {
			const float RScale = 1.f / Scale;
			return Vector3(X * RScale, Y * RScale, Z * RScale);
		}

		__forceinline Vector3 operator+(float A) const {
			return Vector3(X + A, Y + A, Z + A);
		}

		__forceinline Vector3 operator-(float A) const {
			return Vector3(X - A, Y - A, Z - A);
		}

		__forceinline Vector3 operator*(const Vector3& V) const {
			return Vector3(X * V.X, Y * V.Y, Z * V.Z);
		}

		__forceinline Vector3 operator/(const Vector3& V) const {
			return Vector3(X / V.X, Y / V.Y, Z / V.Z);
		}

		__forceinline float operator|(const Vector3& V) const {
			return X * V.X + Y * V.Y + Z * V.Z;
		}

		__forceinline float operator^(const Vector3& V) const {
			return X * V.Y - Y * V.X - Z * V.Z;
		}

		__forceinline Vector3& operator+=(const Vector3& v) {
			X += v.X;
			Y += v.Y;
			Z += v.Z;
			return *this;
		}

		__forceinline Vector3& operator-=(const Vector3& v) {
			X -= v.X;
			Y -= v.Y;
			Z -= v.Z;
			return *this;
		}

		__forceinline Vector3& operator*=(const Vector3& v) {
			X *= v.X;
			Y *= v.Y;
			Z *= v.Z;
			return *this;
		}

		__forceinline Vector3& operator/=(const Vector3& v) {
			X /= v.X;
			Y /= v.Y;
			Z /= v.Z;
			return *this;
		}

		__forceinline bool operator==(const Vector3& src) const {
			return (src.X == X) && (src.Y == Y) && (src.Z == Z);
		}

		__forceinline bool operator!=(const Vector3& src) const {
			return (src.X != X) || (src.Y != Y) || (src.Z != Z);
		}

		__forceinline Vector3 Rotate(float angle)
		{
			return Vector3(X * cos(-angle) - Z * sin(-angle), Y, X * sin(-angle) + Z * cos(-angle));
		}

		__forceinline float Size() const {
			return sqrt(X * X + Y * Y + Z * Z);
		}

		float get_length()
		{
			float ret = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
			return max(ret, 0.000000000000000000000000000000001);
		}

		__forceinline float DistTo(Vector3 targetTo) const {
			return (targetTo - *this).Size();
		}

		__forceinline Vector3 toRotator(Vector3 targetTo)
		{
			Vector3 Normalized = (targetTo - *this);
			return Normalized * (1 / Normalized.Size());
		}
	};
	struct Color
	{
		uint8_t A{ 255 };
		uint8_t R{ 255 };
		uint8_t G{ 255 };
		uint8_t B{ 255 };
		Color(int r, int g, int b, int a = 255) : R(r), G(g), B(b), A(a) {};
		Color(float Input[3]) : R((uint8_t)(Input[0] * 255.f)), G((uint8_t)(Input[1] * 255.f)), B((uint8_t)(Input[2] * 255.f)) {};
		Color(XMFLOAT3 Input) : R((uint8_t)(Input.x * 255.f)), G((uint8_t)(Input.y * 255.f)), B((uint8_t)(Input.z * 255.f)) {};
		DWORD C2D() { return ((A & 0xff) << 24) | ((B & 0xff) << 16) | ((G & 0xff) << 8) | ((R & 0xff)); }
		DWORD ApplyAlpha(int Alpha)
		{
			return ((Alpha & 0xff) << 24) | ((R & 0xff)) | ((G & 0xff) << 8) | ((B & 0xff) << 16);
		}

		DWORD RGBA2ARGB(int Alpha)
		{
			return ((Alpha & 0xff) << 24) | ((B & 0xff)) | ((G & 0xff) << 8) | ((R & 0xff) << 16);
		}
	};
	class Matrix : public XMMATRIX
	{
	public:
		float m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44;

		XMFLOAT3 get_location()
		{
			__try
			{
				XMMATRIX invView = XMMatrixInverse(NULL, (XMMATRIX)*this);
				return XMFLOAT3(XMVectorGetX(invView.r[3]) / XMVectorGetW(invView.r[3]), XMVectorGetY(invView.r[3]) / XMVectorGetW(invView.r[3]), XMVectorGetZ(invView.r[3]) / XMVectorGetW(invView.r[3]));
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
			return XMFLOAT3(0, 0, 0);
		}

		XMFLOAT3 get_rotation()
		{
			__try {
				return XMFLOAT3(XMVectorGetZ(r[0]), XMVectorGetZ(r[1]), XMVectorGetZ(r[2]));
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
			return XMFLOAT3(0, 0, 0);
		}

		Vector3 GetCameraVec()
		{
			float	A = m22 * m33 - m32 * m23,
				B = m32 * m13 - m12 * m33,
				C = m12 * m23 - m22 * m13,
				Z = m11 * A + m21 * B + m31 * C;

			if (abs(Z) < 0.0001) return Vector3();

			float	D = m31 * m23 - m21 * m33,
				E = m11 * m33 - m31 * m13,
				F = m21 * m13 - m11 * m23,
				G = m21 * m32 - m31 * m22,
				H = m31 * m12 - m11 * m32,
				K = m11 * m22 - m21 * m12;

			return Vector3(-(A * m41 + D * m42 + G * m43) / Z, -(B * m41 + E * m42 + H * m43) / Z, -(C * m41 + F * m42 + K * m43) / Z);
		}

		bool WorldToScreen(Vector3 worldPos, Vector2* OutPos, Vector2 WindowSize, bool ignoreret = false)
		{
			float screenX = (r[0].m128_f32[0] * worldPos.X) + (r[1].m128_f32[0] * worldPos.Y) + (r[2].m128_f32[0] * worldPos.Z) + r[3].m128_f32[0];
			float screenY = (r[0].m128_f32[1] * worldPos.X) + (r[1].m128_f32[1] * worldPos.Y) + (r[2].m128_f32[1] * worldPos.Z) + r[3].m128_f32[1];
			float screenW = (r[0].m128_f32[3] * worldPos.X) + (r[1].m128_f32[3] * worldPos.Y) + (r[2].m128_f32[3] * worldPos.Z) + r[3].m128_f32[3];

			float camX = WindowSize.X / 2.0f;
			float camY = WindowSize.Y / 2.0f;
			float x = camX + (camX * screenX / screenW);
			float y = camY - (camY * screenY / screenW);

			if (x < 0 || y < 0 || x >= WindowSize.X || y >= WindowSize.Y) { return false; }

			*OutPos = { x, y };

			return (screenW > 0.001f) || ignoreret;
		}

		Vector2 WorldToScreen(Vector3 worldPos)
		{
			float screenX = (r[0].m128_f32[0] * worldPos.X) + (r[1].m128_f32[0] * worldPos.Y) + (r[2].m128_f32[0] * worldPos.Z) + r[3].m128_f32[0];
			float screenY = (r[0].m128_f32[1] * worldPos.X) + (r[1].m128_f32[1] * worldPos.Y) + (r[2].m128_f32[1] * worldPos.Z) + r[3].m128_f32[1];
			float screenW = (r[0].m128_f32[3] * worldPos.X) + (r[1].m128_f32[3] * worldPos.Y) + (r[2].m128_f32[3] * worldPos.Z) + r[3].m128_f32[3];

			//float camX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
			//float camY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;

			//float x = camX + (camX * screenX / screenW);
			//float y = camY - (camY * screenY / screenW);
			if (screenW <= 0.1f)
				screenW = 0.1f;

			float camX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
			float camY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;

			float x = camX + (camX * screenX / screenW) + screenX / screenW;
			float y = camY - (camY * screenY / screenW) + screenY / screenW;

			if (x < 0 || y < 0 || x >= GetSystemMetrics(SM_CXSCREEN) || y >= GetSystemMetrics(SM_CYSCREEN)) { return Vector2{}; }

			return { x ,y };
		}
	};
}
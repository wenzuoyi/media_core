#ifndef MOUSE_LOCATOR_H_
#define MOUSE_LOCATOR_H_
#include <functional>
class MouseLocator {
public:
	MouseLocator();
	virtual ~MouseLocator();
	void Enable(bool enable);
	bool IsEnable() const;
	void Visible(bool visible);
	bool IsVisible() const;
	void Start(const CPoint& point);
	void Update(const CPoint& point);
	void Transform(std::function<void(const CPoint& left_top, const CPoint& right_bottom)> transformer) const;
	CRect GetRegion() const;
private:
  static int MinValue(int left, int right);
  static int MaxValue(int left, int right);
	CPoint begin_point_;
	CPoint end_point_;
	bool enable_{ false };
	bool visible_{ false };
};
#endif // MOUSE_LOCATOR_H_

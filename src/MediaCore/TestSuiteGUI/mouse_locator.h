#ifndef MOUSE_LOCATOR_H_
#define MOUSE_LOCATOR_H_
#include <functional>
class MouseLocator final {
public:
	MouseLocator();
	virtual ~MouseLocator();
	void Enable(bool enable);
	bool IsEnable() const;
	void Visible(bool visible);
	bool IsVisible() const;
	void Start(const CPoint& point);
	void Update(const CPoint& point, std::function<bool()> condition);
	void Stop(const CPoint& point);
	void Draw(HDC hdc, std::function<CRect(const CPoint& left_top, const CPoint& right_bottom)> transformer) const;
	void Transform(std::function<void(const CPoint& left_top, const CPoint& right_bottom)> transformer) const;
private:
  static int MinValue(int left, int right);
  static int MaxValue(int left, int right);
	CPoint begin_point_;
	CPoint end_point_;
	bool enable_{ false };
	bool visible_{ false };
  HCURSOR arrow_style_cursor_;
  HCURSOR cross_style_cursor_;
  CPen pen_;
  CBrush* empty_brush_{ nullptr };
};
#endif // MOUSE_LOCATOR_H_

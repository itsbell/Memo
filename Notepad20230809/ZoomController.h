//ZoomController.h
#ifndef _ZOOMCONTROLLER_H
#define _ZOOMCONTROLLER_H

class ZoomController {
	
	public:
		ZoomController();
		~ZoomController();

		void ZoomIn();
		void ZoomOut();
		void ZoomSet(float magnification);
		void Reset();
		float GetMagnification() const;

	private:
		float magnification;
};

inline float ZoomController::GetMagnification() const {
	return this->magnification;
}

#endif //_ZOOMCONTROLLER_H
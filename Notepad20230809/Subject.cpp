//Subject.cpp
#include "Subject.h"
#include "Observer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
클래스명: Subject
기    능: 관계 TextEditor -▷ Subject // 관련 디자인패턴(Observer 디자인패턴)
*/

Subject::Subject(Long capacity)
	:observers(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

Subject::~Subject() {

}

/*
함수명칭: Attach
기    능: Array<Observer*> observers의 front에 observer Link를 추가하다.
입    력: Observer Link
출    력: 위치
*/
Long Subject::Attach(Observer* observer) {
	Long index;

	if (this->length < this->capacity) {
		index = this->observers.Store(this->length, observer);
	}

	else {
		index = this->observers.AppendFromRear(observer);
		this->capacity++;
	}

	this->length++;

	return index;
}

/*
함수명칭: Detach
기    능: Array<Observer*> observers의 front에 index번째 값을 지우다.
입    력: Observer Link
출    력: 위치
*/
Long Subject::Detach(Observer* observer) {
	Long index;

	index = this->observers.LinearSearchUnique(observer, CompareObservers);
	
	if (index != -1) {
		index = this->observers.Delete(index);
		this->length--;
		this->capacity--;
	}
	
	return index;
}

int CompareObservers(void* one, void* other) {
	Observer** one_ = (Observer**)one;
	Observer* other_ = (Observer*)other;
	int ret = -1;

	if (*one_ == other) {
		ret = 0;
	}

	return ret;
}

/*
함수명칭: Notify
기    능: 배열에 있는 Observer 클래스들을 모두 업데이트 하다.
입    력: 없음
출    력: 없음
*/
void Subject::Notify() {
	Long i = 0;

	while (i < this->length) {
		this->observers.GetAt(i)->Update(this);
		i++;
	}
}
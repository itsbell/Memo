//Subject.cpp
#include "Subject.h"
#include "Observer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: Subject
��    ��: ���� TextEditor -�� Subject // ���� ����������(Observer ����������)
*/

Subject::Subject(Long capacity)
	:observers(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

Subject::~Subject() {

}

/*
�Լ���Ī: Attach
��    ��: Array<Observer*> observers�� front�� observer Link�� �߰��ϴ�.
��    ��: Observer Link
��    ��: ��ġ
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
�Լ���Ī: Detach
��    ��: Array<Observer*> observers�� front�� index��° ���� �����.
��    ��: Observer Link
��    ��: ��ġ
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
�Լ���Ī: Notify
��    ��: �迭�� �ִ� Observer Ŭ�������� ��� ������Ʈ �ϴ�.
��    ��: ����
��    ��: ����
*/
void Subject::Notify() {
	Long i = 0;

	while (i < this->length) {
		this->observers.GetAt(i)->Update(this);
		i++;
	}
}
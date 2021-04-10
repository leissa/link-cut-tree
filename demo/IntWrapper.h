#ifndef INT_WRAPPER_H
#define INT_WRAPPER_H

class IntWrapper {
private:
	int _sizeVirtual;
	int _sizeSubtree;
public:
	int _value;
	IntWrapper();
	IntWrapper(int aValue);
	void update_aggregate(IntWrapper* aLeft, IntWrapper* aRight);
	void update_aggregate_expose(IntWrapper* aNewChild, IntWrapper* aFormerChild);
	void update_aggregate_link(IntWrapper* aNewChild);
	int getSubtreeSize();
	bool operator<(const IntWrapper& aOther) const noexcept;
};

IntWrapper::IntWrapper() :_value(0), _sizeVirtual(0), _sizeSubtree(0) {}

IntWrapper::IntWrapper(int aValue) :_value(aValue), _sizeVirtual(0), _sizeSubtree(0) {}

void IntWrapper::update_aggregate(IntWrapper* left, IntWrapper* right) {
	_sizeSubtree = (left ? left->_sizeSubtree : 0) + (right ? right->_sizeSubtree : 0) + _sizeVirtual + 1;
}

void IntWrapper::update_aggregate_expose(IntWrapper* aNewChild, IntWrapper* aFormerChild) {
	if (aNewChild) {
		_sizeVirtual -= aNewChild->_sizeSubtree;
	}
	if (aFormerChild) {
		_sizeVirtual += aFormerChild->_sizeSubtree;
	}
}

void IntWrapper::update_aggregate_link(IntWrapper* newChild) {
	_sizeVirtual += newChild->_sizeSubtree;
}

inline bool IntWrapper::operator<(const IntWrapper& aOther) const noexcept
{
	return aOther._value > _value;
}

int IntWrapper::getSubtreeSize()
{
	return _sizeSubtree;
}

#endif
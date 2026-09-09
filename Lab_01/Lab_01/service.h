
#include <string>
class Service {
private:
	int id;
	std::string name;
	float price;
	int length;
	static int counter;
public:
	Service(const std::string& nameV, float priceV, int lengthV);	
	int getId()const;
	std::string getName() const;
	float getPrice()const;
	int getLength()const;
	void setPrice(float priceValue);
	void setLength(int lengthValue);
	void printInfo()const;

};

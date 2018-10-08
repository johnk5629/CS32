//Your declarations and implementations would go here

class Investment
{
  public:
	  Investment(string nm, int price);
	  virtual ~Investment();
	  string name() const;
	  virtual bool fungible() const;
	  virtual string description() const = 0;
	  int purchasePrice() const;

  private:
	  string m_name;
	  int m_price;
};

class Painting :  public Investment
{
  public:
	  Painting(string artist, int price);
	  virtual ~Painting();
	  virtual string description() const;

  private:

};

class Stock : public Investment
{
  public:
	  Stock(string nm, int price, string ts);
	  virtual ~Stock();
	  virtual bool fungible() const;
	  virtual string description() const;

  private:
	  string m_ticker;
};

class House : public Investment
{
  public:
	  House(string address, int price);
	  virtual ~House();
	  virtual string description() const;

  private:
};

//Investment member functions (non virtual + constructor)
Investment::Investment(string nm, int price)
	: m_name(nm), m_price(price)
{}

Investment::~Investment()
{}

string Investment::name() const
{
	return m_name;
}

bool Investment::fungible() const
{
	return false;
}

int Investment::purchasePrice() const
{
	return m_price;
}

//Painting
Painting::Painting(string artist, int price)
	: Investment(artist, price)
{}

Painting::~Painting()
{
	cout << "Destroying " << Investment::name() << ", a painting" << endl;
}

string Painting::description() const
{
	return "painting";
}

//Stock
Stock::Stock(string nm, int price, string ts)
	: Investment(nm, price), m_ticker(ts)
{}

Stock::~Stock()
{
	cout << "Destroying " << Investment::name() << ", a stock holding" << endl;
}

bool Stock::fungible() const
{
	return true;
}

string Stock::description() const
{
	return "stock trading as " + m_ticker;
}

//House
House::House(string address, int price)
	: Investment(address, price)
{}

House::~House()
{
	cout << "Destroying the house " << Investment::name() << endl;
}

string House::description() const
{
	return "house";
}
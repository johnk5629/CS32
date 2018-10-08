void listAll(const Class* c, string path)  // two-parameter overload
{
	cout << path + c->name() << endl;

	if (c->subclasses().empty())
		return;

	for (int i = 0; i < c->subclasses().size(); i++)
	{
		listAll(c->subclasses()[i], path + c->name() + "=>");
	}

}
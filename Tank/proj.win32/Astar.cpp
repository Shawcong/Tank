#include "Astar.h"
#include "Astaritem.h"

Astar::Astar(void)
{
}

Astar::~Astar(void)
{
    open->removeAllObjects();
    close->removeAllObjects();
    path->removeAllObjects();
}
__Array *Astar::findPath(int curX, int curY, int aimX, int aimY, TMXTiledMap* passmap,Tank *x)
{
	log("###############");
	curCol = curX;
    curRow = curY;
	aimCol = aimX;
	aimRow = aimY;
	map = passmap;
	collidable = map->getLayer("bg1");
	tank = x;
	path = __Array::create();
	open = __Array::create();
	AstarItem * temp = new AstarItem();
	open->addObject(temp);
	AstarItem * temp1 = new AstarItem();
	temp1->setpos(curCol,curRow);
	temp1->setg(0);
	int ag = getH(curCol,curRow);
	temp1->seth(ag);
	temp1->setfid(0);
	temp1->setf(ag);
	open->addObject(temp1);
	close = __Array::create();
	while (open->count() > 1) {
		//寻找起点A周围可以到达的方格, 将它们放入"开启列表", 并设置它们的"父方格"为A.
		//从"开启列表"中删除起点 A, 并将起点 A 加入"关闭列表", "关闭列表"中存放的都是不需要再次检查的方格
		fromopentoclose();
		int fatherid = close->count() - 1;//close列表的最后一个元素
		// "开始列表" 里出现了目标终点方块的时候, 说明路径已经被找到.
		int coldelta = abs(aimCol - ((AstarItem *)close->getObjectAtIndex(fatherid))->getcol());
		int rowdelta = abs(aimRow - ((AstarItem *)close->getObjectAtIndex(fatherid))->getrow());
		if (coldelta <= 1 && rowdelta <= 1) {
			getpath();
			log("success");
			break;
		}
		else {
			starseach(fatherid);
		}
	}
	open->removeAllObjects();
	close->removeAllObjects();
	if (path->count() == 0) {
		AstarItem * temp = new AstarItem();
		temp->setpos(aimCol, aimRow);
		path->addObject(temp);
		return path;
	}
	else {
		if (((AstarItem *)path->getLastObject())->getcol() != aimCol || ((AstarItem *)path->getLastObject())->getrow() != aimRow) {
			AstarItem * temp = new AstarItem();
			temp->setpos(aimCol, aimRow);
			path->addObject(temp);
		}
		return path;
	}
}

void Astar::starseach(int fid)
{
	//检查它所有相邻并且可以到达(障碍物和 "关闭列表" 的方格都不考虑) 的方格.如果这些方格还不在 "开启列表" 里的话, 
	//将它们加入 "开启列表", 计算这些方格的 G, H 和 F 值各是多少, 并设置它们的 "父方格" 为 C.
	//如果某个相邻方格 D 已经在 "开启列表" 里了, 检查如果用新的路径 (就是经过C 的路径) 到达它的话, 
	//G值是否会更低一些, 如果新的G值更低, 那就把它的 "父方格" 改为目前选中的方格 C, 然后重新计算它的 F 值和 G 值
	//(H 值不需要重新计算, 因为对于每个方块, H 值是不变的). 如果新的 G 值比较高, 就说明经过 C 再到达 D 不是一个明智的选择, 
	//因为它需要更远的路, 这时我们什么也不做.
	int col = ((AstarItem *)close->getObjectAtIndex(fid))->getcol();
	int row = ((AstarItem *)close->getObjectAtIndex(fid))->getrow();
	int mycol = col;
	int myrow = row - 3;
	if (myrow>= 0 && checkmap(mycol, myrow)) {
		if (checkOpen(mycol, myrow, fid) && checkclose(mycol, myrow)) {
			addtoopen(mycol, myrow, fid);
		}
	}
	mycol = col - 3;
	myrow = row;
	if (mycol>= 0 && checkmap(mycol, myrow)) {
		if (checkOpen(mycol, myrow, fid) && checkclose(mycol, myrow)) {
			addtoopen(mycol, myrow, fid);
		}
	}
	mycol = col;
	myrow = row + 3;
	if (myrow + tank->getContentSize().width < map->getContentSize().width && checkmap(mycol, myrow)) {
		if (checkOpen(mycol, myrow, fid) && checkclose(mycol, myrow)) {

			addtoopen(mycol, myrow, fid);
		}
	}
	mycol = col + 3;
	myrow = row;
	if (mycol + tank->getContentSize().height < map->getContentSize().height && checkmap(mycol, myrow)) {
		if (checkOpen(mycol, myrow, fid) && checkclose(mycol, myrow)) {
			addtoopen(mycol, myrow, fid);
		}
	}
}

bool Astar::checkmap(int col, int row)
{
	int tileGid =0;
	for (int i = col; i <= col + tank->getContentSize().width; i++) {
		for (int j = row; j <= row + tank->getContentSize().height; j++) {
			tileGid = collidable->getTileGIDAt(tileCoordFromPosition(Point(i, j)));
			if (tileGid > 0) {
				return false;
			}
		}
	}
	if (tileGid == 0) {
		return true;
	}
}

void Astar::addtoopen(int col, int row, int id)
{
	AstarItem * temp = new AstarItem();
	temp->setpos(col, row);
	temp->setfid(id);
	int g = getG(col, row, id);
	int h = getH(col, row);
	temp->setg(g);
	temp->seth(h);
	temp->setf(g + h);
	open->addObject(temp);
	resetSort(open->count() - 1);

}

bool Astar::checkclose(int col, int row)
{
	for (int i = close->count() - 1; i > 0; i--) {
		if (((AstarItem *)close->getObjectAtIndex(i))->getcol() == col && ((AstarItem *)close->getObjectAtIndex(i))->getrow() == row) {
			return false;
		}
	}
	return true;
}

bool Astar::checkOpen(int col, int row, int id)
{
	for (int i = open->count() - 1; i > 0; i--) {
		if (((AstarItem *)open->getObjectAtIndex(i))->getcol() == col && ((AstarItem *)open->getObjectAtIndex(i))->getrow() == row) {
			int tempG = getG(col, row, id);
			if (tempG < ((AstarItem *)open->getObjectAtIndex(i))->getg()) {
				((AstarItem *)open->getObjectAtIndex(i))->setg(tempG);
				((AstarItem *)open->getObjectAtIndex(i))->setfid(id);
				((AstarItem *)open->getObjectAtIndex(i))->setf(tempG + ((AstarItem *)open->getObjectAtIndex(i))->geth());
				resetSort(i);
			}
			return false;
		}
	}
	return true;
}

void Astar::getpath()
{
	curCol = aimCol;
	curRow = aimRow;
	if (close->count() == 0) {
		AstarItem * temp = new AstarItem();
		temp->setpos(aimCol, aimRow);
		path->addObject(temp);
		return;
	}
	if (path->count() == 0) {
		path->addObject(close->getObjectAtIndex(close->count() - 1));
	}
	else {
		path->insertObject(close->getObjectAtIndex(close->count() - 1), path->count() - 1);
	}
	while (true) {
		if (((AstarItem *)path->getObjectAtIndex(0))->getg() == 0) {
			break;
		}
		path->insertObject(close->getObjectAtIndex(((AstarItem *)path->getObjectAtIndex(0))->getfid()), 0);
	}
}

void Astar::fromopentoclose()
{
	AstarItem * temp = (AstarItem *)open->getObjectAtIndex(1);
	close->addObject(temp);
	open->removeObjectAtIndex(1);
	log("#############");
	int i = temp->getcol();
	int j = temp->getrow();
	int k = close->count();
	int z = map->getMapSize().height;
}
void Astar::removefromopen()
{
}
void Astar::resetSort(int last)
{
	while (last > 1) {
		int half = last / 2;
		if (((AstarItem *)open->getObjectAtIndex(half))->getf() <= ((AstarItem *)open->getObjectAtIndex(last))->getf())
			break;
		open->exchangeObjectAtIndex(half, last);
		last = half;
	}
}
int Astar::getG(int col, int row, int id)
{
	int fx = ((AstarItem *)close->getObjectAtIndex(id))->getcol();
	int fy = ((AstarItem *)close->getObjectAtIndex(id))->getrow();
	int fg = ((AstarItem *)close->getObjectAtIndex(id))->getg();
	return fg + 10;
}
int Astar::getH(int col, int row)
{
	return abs(aimCol - col) * 10 + abs(aimRow - row) * 10;
}

//实现从像素点坐标转化为瓦片坐标
Point Astar::tileCoordFromPosition(Point pos)
{
	int x = pos.x / map->getTileSize().width;
	int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
	return Point(x, y);
}
--此lua应该是装备镶嵌和融合C相关，丸子看不懂!
function getIntPart(x)
    if x <= 0 then
       return math.ceil(x);
    end
    if math.ceil(x) == x then
       return math.ceil(x);
    else
       return math.ceil(x) - 1;
    end
end

function GetTtileBuff(id)
	if id >96 or id < 1 then
		return 0,0,0,0
	end
	return titleinfo[id][3],titleinfo[id][4],titleinfo[id][5],titleinfo[id][6],titleinfo[id][7],titleinfo[id][8],titleinfo[id][9],titleinfo[id][10],titleinfo[id][11]
end

function FreeComplianceParameter( charaindex )
	if char.getInt(charaindex, "类型") == 3 then
		local data= char.getChar(charaindex,"祝福状态")
		if data~="" then
			local hp = other.atoi(other.getString(data, "|", 1))
			local at = other.atoi(other.getString(data, "|", 2))
			local df = other.atoi(other.getString(data, "|", 3))
			local dx = other.atoi(other.getString(data, "|", 4))

			char.setWorkInt(charaindex, "最大HP", char.getWorkInt(charaindex, "最大HP") + hp)
			char.setWorkInt(charaindex, "攻击力", char.getWorkInt(charaindex, "攻击力") + at)
			char.setWorkInt(charaindex, "防御力", char.getWorkInt(charaindex, "防御力") + df)
			char.setWorkInt(charaindex, "敏捷力", char.getWorkInt(charaindex, "敏捷力") + dx)

		end
	end



	if char.getInt(charaindex, "类型") == 1 then
		local itemindex =  char.getItemIndex(charaindex,6)
		if itemindex > 0 then
			local data1 = item.getChar(itemindex, "字段")
			local attrate = other.atoi(other.getString(data1, "|", 1))
			local defrate = other.atoi(other.getString(data1, "|", 2))
			local dexrate = other.atoi(other.getString(data1, "|", 3))
			char.setWorkInt(charaindex, "攻击力", math.floor(char.getWorkInt(charaindex, "攻击力") * (100+attrate)/100))
			char.setWorkInt(charaindex, "防御力", math.floor(char.getWorkInt(charaindex, "防御力") * (100+defrate)/100))
			char.setWorkInt(charaindex, "敏捷力", math.floor(char.getWorkInt(charaindex, "敏捷力") * (100+dexrate)/100))
			char.setWorkInt(charaindex, "最大HP", char.getWorkInt(charaindex, "最大HP") + 150)
		end

		for i = 1,96 do
			if char.getCharNewTitleMode(charaindex,i-1)== 1 then
				temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9 = GetTtileBuff(i)
				addhp = addhp + temp1
				addat = addat + temp2
				adddf = adddf + temp3
				adddx = adddx + temp4
				addpo = addpo + temp5
				addpa = addpa + temp6
				addsl = addsl + temp7
				addst = addst + temp8
				addco = addco + temp9
			end
		end
		char.setWorkInt(charaindex, "最大HP", char.getWorkInt(charaindex, "最大HP") + addhp)
		char.setWorkInt(charaindex, "攻击力", char.getWorkInt(charaindex, "攻击力") + addat)
		char.setWorkInt(charaindex, "防御力", char.getWorkInt(charaindex, "防御力") + adddf)
		char.setWorkInt(charaindex, "敏捷力", char.getWorkInt(charaindex, "敏捷力") + adddx)
		char.setWorkInt(charaindex, "毒抗附加", addpo)
		char.setWorkInt(charaindex, "麻抗附加", addpa)
		char.setWorkInt(charaindex, "睡抗附加", addsl)
		char.setWorkInt(charaindex, "石抗附加", addst)
		char.setWorkInt(charaindex, "混抗附加", addco)

	end

end

function data()
	titleinfo =	{	--描述 序号 体 攻 防 敏 毒 麻 睡 石 混
				 {"大地的精灵 防+5",1,0,0,5,0,0,0,0,0,0}
				,{"水的精灵 HP+20",2,20,0,0,0,0,0,0,0,0}
				,{"火炎的精灵 攻+5",3,0,5,0,0,0,0,0,0,0}
				,{"疾风的精灵 敏+5",4,0,0,0,5,0,0,0,0,0}
				,{"奥萝拉的精灵 全+1",5,1,1,1,1,0,0,0,0,0}
				,{"彩虹的精灵 全+2",6,2,2,2,2,0,0,0,0,0}
				,{"初心使者 HP+10",7,10,0,0,0,0,0,0,0,0}
				,{"无名的旅人 全+2",8,2,2,2,2,0,0,0,0,0}
				,{"路边的落叶 全+4",9,4,4,4,4,0,0,0,0,0}
				,{"水面的小草 全+6",10,6,6,6,6,0,0,0,0,0}
				,{"呢喃的歌声 全+8",11,8,8,8,8,0,0,0,0,0}
				,{"地上的月影 全+10",12,10,10,10,10,0,0,0,0,0}
				,{"奔跑的春风 全+12",13,12,12,12,12,0,0,0,0,0}
				,{"苍穹的风云 全+14",14,14,14,14,14,0,0,0,0,0}
				,{"摇曳的金星 全+16",15,16,16,16,16,0,0,0,0,0}
				,{"欢喜的慈雨 全+18",16,18,18,18,18,0,0,0,0,0}
				,{"蕴含的太阳 全+20",17,20,20,20,20,0,0,0,0,0}
				,{"敬畏的寂静 全+30",18,30,30,30,30,0,0,0,0,0}
				,{"精靈封印者 攻+5",19,0,5,0,0,0,0,0,0,0}
				,{"五转轮回 防+20",20,0,0,20,0,0,0,0,0,0}
				,{"盗贼讨伐者 攻+5",21,0,5,0,0,0,0,0,0,0}
				,{"盗贼帮众 混乱抗性+5",22,0,0,0,0,0,0,0,0,5}
				,{"夺宝奇兵 石化抗性+5",23,0,0,0,0,0,0,0,5,0}
				,{"勇闯夺宝岛 睡抗性+5",24,0,0,0,0,0,0,5,0,0}
				,{"合成达人 敏+5",25,0,0,0,5,0,0,0,0,0}
				,{"合成高手 敏+3",26,0,0,0,3,0,0,0,0,0}
				,{"练宠达人 攻+5",27,0,5,0,0,0,0,0,0,0}
				,{"练宠高手 攻+3",28,0,3,0,0,0,0,0,0,0}
				,{"任务达人 防+5",29,0,0,5,0,0,0,0,0,0}
				,{"任务高手 防+3",30,0,0,3,0,0,0,0,0,0}
				,{"初犊战士 HP+10",31,10,0,0,0,0,0,0,0,0}
				,{"圣灵勇士 HP+15",32,15,0,0,0,0,0,0,0,0}
				,{"玛丽那丝英雄 HP+20",33,20,0,0,0,0,0,0,0,0}
				,{"萨姆吉尔英雄 HP+25",34,25,0,0,0,0,0,0,0,0}
				,{"萨伊那斯使者 HP+30",35,30,0,0,0,0,0,0,0,0}
				,{"尼斯大陆英雄 HP+100",36,100,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				}




end

function main()
	data()
end

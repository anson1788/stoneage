

function FreeGetBattleMap(floor)
	math.randomseed(os.time());
	local ret=0;
	for i=1,table.getn(BattleMapList) do
		if floor >= BattleMapList[i][1][1] and floor <= BattleMapList[i][1][2] then
			local index = math.mod(math.random(9999,999999),table.getn(BattleMapList[i][2]));
			if index == 0 then
				index = table.getn(BattleMapList[i][2]);
			end
			ret = BattleMapList[i][2][index];
			break;
		end
	end
	return ret;
end

function main()
BattleMapList = {
									{{60403,60403},{59301}},
									{{33001,33020},{59314}},
									{{33021,33040},{59315}},
									{{33041,33060},{59316}},
									{{33060,33080},{59317}},
									{{33081,33101},{59318}},
									{{35001,35016},{59300}},
								};--{开始地图号,结束地图号},{随机战斗地图1,随机战斗地图2...随机战斗地图n}
end

function data()
end

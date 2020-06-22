function FreeSaMenu(meindex, index)
--	char.TalkToCli(meindex, -1, char.getWorkInt(meindex,"NPC临时3"), "绿色")
	local 战模 = char.getWorkInt(meindex,"战斗");
	parameter = {meindex}
	if index == 11 then
		if 战模 ==0 then
			char.Encounter(meindex)
			char.TalkToCli(meindex, -1, "开启原地遇敌！", "红色")
		else
			char.TalkToCli(meindex, -1, "您已经战斗中！", "红色")
		end
	elseif index == 12 then
		char.ClearEncounter(meindex);
		char.TalkToCli(meindex, -1, "关闭原地遇敌！", "红色")
	elseif index == 13 then
		if 战模 == 0 then
			other.CallFunction("CheckTalked", "data/ablua/npc/check/check.lua", parameter)
		end
	--	other.CallFunction("ShowHead", "data/ablua/npc/vipshop/vipshop.lua", parameter)
	elseif index == 14 then
			other.CallFunction("QueryMissionTalked", "data/ablua/npc/querymission/querymission.lua", parameter)
	elseif index == 16 then
		other.CallFunction("FreeDataInfoShow", "data/ablua/freedatainfoshow.lua", parameter)
	elseif index == 17 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es")
	elseif index == 18 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/forum.php")
	elseif index == 19 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/pay.php")
	elseif index == 20 then
		char.setWorkInt(meindex,"NPC临时3",1)
		other.CallFunction("OnlineService", "data/ablua/npc/onlineservice/onlineservice.lua", parameter)
	elseif index == 21 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/forum.php?mod=viewthread&tid=54&extra=page%3D1")
	elseif index == 22 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/bbs/forum.php?mod=viewthread&tid=226")
	elseif index == 30 then--切换队长
		if char.getWorkInt(meindex, "组队") == 1 then
			转移动伍[meindex]={};
			local 队员名 = "";
			local 计数=0;
			for i=1,4 do
				local 队员索引 = char.getWorkInt(meindex, "队员" .. i+1)
				if char.check(队员索引) == 1 then
					计数 = 计数 + 1;
					转移动伍[meindex][计数] = 队员索引;
					队员名 = string.format("%s        %s\n",队员名,char.getChar(转移动伍[meindex][计数],"名字"));
				end
			end
			local 内容 = "1 						请选择新的队长\n"..队员名;
			lssproto.windows(meindex, 2, 8, 0, char.getWorkInt( npcindex, "对象"), 内容)
		else
			char.TalkToCli(meindex, -1, "该功能需要队长权限", "红色")
		end
	else
		char.TalkToCli(meindex, -1, "暂未开放", "红色")
	end
end


function WindowTalked( NPC索引, 玩家索引, 序号, 按钮, 数据)
	if 序号 == 0 then
		local 选项 = tonumber(数据);
		if 选项 > 0 and 选项 < 5 then
			char.DischargeParty(玩家索引,0);
			char.JoinParty(转移动伍[玩家索引][选项],玩家索引,0);
			char.TalkToCli(转移动伍[玩家索引][选项], -1, "您的队友【"..char.getChar(玩家索引,"名字").."】把队长权力交给您！", "黄色")
			char.TalkToCli(玩家索引, -1, "您成功把队长权力交给队友【"..char.getChar(转移动伍[玩家索引][选项],"名字").."】", "黄色");
			for i =1,table.getn(转移动伍[玩家索引]) do
				if i ~= 选项 then
					char.JoinParty(转移动伍[玩家索引][选项],转移动伍[玩家索引][i],0);
				end
			end
			char.Encounter(转移动伍[玩家索引][选项])
		end
	end
end

function Create(name, metamo, floor, x, y, dir)
	npcindex = npc.CreateNpc(name, metamo, floor, x, y, dir)
	char.setFunctionPointer(npcindex, "窗口事件", "WindowTalked", "")
end

function data()
 转移动伍={};
--[[

11 原地遇敌
12 取消原地
13 支票制作
14 任务查询
16 个人信息
20 卡密使用
21 任务攻略
22 新手帮助


]]


end

function main()
	data()
	Create("查询宠物NPC临时", 100000, 777, 20, 21, 4);
end


记录道具记录={};


function WindowTalked ( meindex, charaindex, seqno, select, data)
	if seqno == 1 then
		local 宠物位置 = other.atoi(data);
		if 宠物位置 >=1 and 宠物位置<=4 then
			local 宠物索引 = char.getCharPet(charaindex, 宠物位置-1);
			if char.check(宠物索引) == 1 then
				记录道具记录[charaindex][3] = 宠物索引;
				local 对话内容 = "是否让 "..char.getChar(宠物索引,"名字") .." 学习该特效！"
				lssproto.windows(charaindex, "对话框", "确定|取消", 2,  char.getWorkInt( npcindex, "对象"),对话内容 )
			end
		end
	elseif seqno == 2 then
		if select == 1 then
			local 攻击特效ID = other.atoi(item.getChar(记录道具记录[charaindex][1], "字段"))
			char.setInt(记录道具记录[charaindex][3],"攻击特效",攻击特效ID);
			char.DelItem(charaindex, 记录道具记录[charaindex][2]);
			char.TalkToCli(charaindex, -1, "您的宠物"..char.getChar(记录道具记录[charaindex][3],"名字").."习得攻击特效,快去体验吧！", "红色")
			记录道具记录[charaindex]=nil;
		end
	end
end

function attackeffect(itemindex, charaindex, toindex, haveitemindex)
	记录道具记录[charaindex]={};
	记录道具记录[charaindex][1]=itemindex;
	记录道具记录[charaindex][2]=haveitemindex;
	lssproto.windows(charaindex, "宠物框", "确定", 1,  char.getWorkInt( npcindex, "对象"), "")
end

function Create(name, metamo, floor, x, y, dir)
	--创建NPC(NPC名字，图像号，地图号，坐标X，坐标Y，方向号)将返回一个NPC索引
	npcindex = npc.CreateNpc(name, metamo, floor, x, y, dir)
	char.setFunctionPointer(npcindex, "窗口事件", "WindowTalked", "")
end


function data()
end

function main()
	Create("攻击特效NPC",100000,777,20,20,3);

	item.addLUAListFunction( "ITEM_ATTACKEFFECT", "attackeffect", "")
end

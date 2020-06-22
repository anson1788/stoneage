function useitemmax(itemindex, charaindex, toindex, haveitemindex)

	local data = other.atoi(item.getChar(itemindex, "字段"))
	if (char.getInt(charaindex,"道具栏状态")~=2 and data == 3)
		or (char.getInt(charaindex,"道具栏状态")~=0 and data == 2) then
		char.TalkToCli(charaindex, -1,"请用相对应的扩展道具来扩展包裹！", "红色")
		return
	end
	if char.getInt(charaindex,"道具栏状态") == 2 then
		char.setInt(charaindex,"道具栏状态",6);
		char.DelItem(charaindex, haveitemindex)
		char.TalkToCli(charaindex, -1,"恭喜您开启第三栏道具栏!", "黄色")
		char.UpCahrData(charaindex,"p");
	elseif char.getInt(charaindex,"道具栏状态") == 6 then
		char.TalkToCli(charaindex, -1,"您已经全部开放了!", "黄色")
	else
		char.DelItem(charaindex, haveitemindex)
		char.setInt(charaindex,"道具栏状态",2);
		char.TalkToCli(charaindex, -1,"恭喜您开启第二栏道具栏!", "黄色")
		char.UpCahrData(charaindex,"p");
	end
end

function main()
	item.addLUAListFunction( "ITEM_USEITEMMAX", "useitemmax", "")
end

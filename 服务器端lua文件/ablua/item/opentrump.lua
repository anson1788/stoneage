function opentrump(itemindex, charaindex, toindex, haveitemindex)

	if char.getInt(charaindex,"法宝状态")>0 then
		char.TalkToCli(charaindex, -1,"您已经开启了法宝装备栏!", "黄色")
		return
	end
	char.DelItem(charaindex, haveitemindex)
	char.setInt(charaindex,"法宝状态",1);
	char.UpCahrData(charaindex,"p");
	char.TalkToCli(charaindex, -1,"您的法宝装备栏已成功开启!", "黄色")
end

function main()
	item.addLUAListFunction( "ITEM_OPENTRUMP", "opentrump", "")
end

function titleuse(itemindex, charaindex, toindex, haveitemindex)
	local id = other.atoi(item.getChar(itemindex, "×Ö¶Î"))
	if GetCharNewTitleMode(charaindex,id -1)== 1 then
		char.TalkToCli(charaindex, -1, "ÄúÒÑÓµÓĞ¸Ã³ÆºÅ£¡", "»ÆÉ«")
		return
	end
	local titlename = item.getChar(itemindex,"ÏÔÊ¾Ãû")
	char.DelItem(charaindex, haveitemindex)
	char.setCharNewTitleMode(charaindex,id -1)
--	char.setInt(charaindex, "³ÆºÅ×´Ì¬2", 0)
---	char.setInt(charaindex, "³ÆºÅ×´Ì¬3", 0)

	char.TalkToCli(charaindex, -1, "¹§Ï²Äã»ñµÃĞÂ³ÆºÅ"..titlename.."£¡", "»ÆÉ«")
	char.talkToServer(-1, "[¹¦³ÉÃû¾Í]¹§Ï²ÓÂÕß " .. char.getChar(charaindex, "Ãû×Ö") .. " »ñµÃ"..titlename.."³ÆºÅ", "»ÆÉ«")
	local param = {charaindex}
	other.CallFunction("TitleListSend", "data/ablua/freechartitle.lua", param)
end

function GetCharNewTitleMode(charaindex,id)
	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬1"),id)~= 0 then
			return 1
		end
	elseif id< 64 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬2"),id-32)~= 0 then
			return 1
		end
	elseif id< 96 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬3"),id-64)~= 0 then
			return 1
		end
	end
	return 0
end

function main()
	item.addLUAListFunction( "ITEM_TITLE", "titleuse", "")
end

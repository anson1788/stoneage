function GetCharNewTitleNo(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end
	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬1"),id)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 64 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬2"),id-32)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 96 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬3"),id-64)~= 0 then
			return titleinfo[id+1][2]
		end
	end
	return 0
end

function GetCharNewTitleString(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end
	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬1"),id)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 64 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬2"),id-32)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 96 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬3"),id-64)~= 0 then
			return titleinfo[id+1][2]
		end
	end
	return ""
end


function GetCharNewTitleName(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end

	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬1"),id)~= 0 then
			return titleinfo[id+1][1]
		end
	elseif (id >=32 and id< 64) then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬2"),id-32)~= 0 then
			return titleinfo[id+1][1]
		end
	elseif(id >=64 and id< 96) then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬3"),id-64)~= 0 then
			return titleinfo[id+1][1]
		end
	end
	return ""
end

function GetCharNewTitleMode(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end

	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬1"),id)~= 0 then
			return 1
		end
	elseif (id >=32 and id< 64) then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬2"),id-32)~= 0 then
			return 1
		end
	elseif(id >=64 and id< 96) then
		if other.DataAndData(char.getInt(charaindex,"³ÆºÅ×´Ì¬3"),id-64)~= 0 then
			return 1
		end
	end
	return 0
end

function TitleListSend(charaindex)
	if char.check(charaindex)~=1 then
		return
	end
	local Nodata=""
	local jiedata=""
	local indexdata=""
	local num=0
	local fd = char.getFd(charaindex)
	for i = 1,96 do
		if GetCharNewTitleMode(charaindex,i-1)>0 then
			num = num + 1
--			print(num, GetCharNewTitleMode(charaindex,i-1))
			Nodata = Nodata..GetCharNewTitleString(charaindex,i-1).."|"
			jiedata = jiedata..GetCharNewTitleName(charaindex,i-1).."|"
			indexdata = indexdata..(i-1).."|"
		end
	end
	local data = string.format("%d|%d#@%s#@%s#@%s#@",char.getInt(charaindex,"ÎÄ×Ö³ÆºÅ"),num,Nodata,jiedata,indexdata)
	lssproto.CharTitleSend(fd,data)
--	print("CharTitleSend·¢ËÍ³É¹¦£¡"..data.."\n")
end

function GetTtileBuff(id)
	if id >96 or id < 1 then
		return 0,0,0,0
	end
	return titleinfo[id][3],titleinfo[id][4],titleinfo[id][5],titleinfo[id][6]
end

function data()
	titleinfo =	{	--ÃèÊö ÐòºÅ Ìå ¹¥ ·À Ãô ¶¾ Âé Ë¯ Ê¯ »ì
				 {"´óµØµÄ¾«Áé ·À+5",1,0,0,5,0,0,0,0,0,0}
				,{"Ë®µÄ¾«Áé HP+20",2,20,0,0,0,0,0,0,0,0}
				,{"»ðÑ×µÄ¾«Áé ¹¥+5",3,0,5,0,0,0,0,0,0,0}
				,{"¼²·çµÄ¾«Áé Ãô+5",4,0,0,0,5,0,0,0,0,0}
				,{"°ÂÂÜÀ­µÄ¾«Áé È«+1",5,1,1,1,1,0,0,0,0,0}
				,{"²ÊºçµÄ¾«Áé È«+2",6,2,2,2,2,0,0,0,0,0}
				,{"³õÐÄÊ¹Õß HP+10",7,10,0,0,0,0,0,0,0,0}
				,{"ÎÞÃûµÄÂÃÈË È«+2",8,2,2,2,2,0,0,0,0,0}
				,{"Â·±ßµÄÂäÒ¶ È«+4",9,4,4,4,4,0,0,0,0,0}
				,{"Ë®ÃæµÄÐ¡²Ý È«+6",10,6,6,6,6,0,0,0,0,0}
				,{"ÄØà«µÄ¸èÉù È«+8",11,8,8,8,8,0,0,0,0,0}
				,{"µØÉÏµÄÔÂÓ° È«+10",12,10,10,10,10,0,0,0,0,0}
				,{"±¼ÅÜµÄ´º·ç È«+12",13,12,12,12,12,0,0,0,0,0}
				,{"²Ôñ·µÄ·çÔÆ È«+14",14,14,14,14,14,0,0,0,0,0}
				,{"Ò¡Ò·µÄ½ðÐÇ È«+16",15,16,16,16,16,0,0,0,0,0}
				,{"»¶Ï²µÄ´ÈÓê È«+18",16,18,18,18,18,0,0,0,0,0}
				,{"ÔÌº¬µÄÌ«Ñô È«+20",17,20,20,20,20,0,0,0,0,0}
				,{"¾´Î·µÄ¼Å¾² È«+30",18,30,30,30,30,0,0,0,0,0}
				,{"¾«ì`·âÓ¡Õß ¹¥+5",19,0,5,0,0,0,0,0,0,0}
				,{"Îå×ªÂÖ»Ø ·À+20",20,0,0,20,0,0,0,0,0,0}
				,{"µÁÔôÌÖ·¥Õß ¹¥+5",21,0,5,0,0,0,0,0,0,0}
				,{"µÁÔô°ïÖÚ »ìÂÒ¿¹ÐÔ+5",22,0,0,0,0,0,0,0,0,5}
				,{"¶á±¦Ææ±ø Ê¯»¯¿¹ÐÔ+5",23,0,0,0,0,0,0,0,5,0}
				,{"ÓÂ´³¶á±¦µº Ë¯¿¹ÐÔ+5",24,0,0,0,0,0,0,5,0,0}
				,{"ºÏ³É´ïÈË Ãô+5",25,0,0,0,5,0,0,0,0,0}
				,{"ºÏ³É¸ßÊÖ Ãô+3",26,0,0,0,3,0,0,0,0,0}
				,{"Á·³è´ïÈË ¹¥+5",27,0,5,0,0,0,0,0,0,0}
				,{"Á·³è¸ßÊÖ ¹¥+3",28,0,3,0,0,0,0,0,0,0}
				,{"ÈÎÎñ´ïÈË ·À+5",29,0,0,5,0,0,0,0,0,0}
				,{"ÈÎÎñ¸ßÊÖ ·À+3",30,0,0,3,0,0,0,0,0,0}
				,{"³õ¶¿Õ½Ê¿ HP+10",31,10,0,0,0,0,0,0,0,0}
				,{"Ê¥ÁéÓÂÊ¿ HP+15",32,15,0,0,0,0,0,0,0,0}
				,{"ÂêÀöÄÇË¿Ó¢ÐÛ HP+20",33,20,0,0,0,0,0,0,0,0}
				,{"ÈøÄ·¼ª¶ûÓ¢ÐÛ HP+25",34,25,0,0,0,0,0,0,0,0}
				,{"ÈøÒÁÄÇË¹Ê¹Õß HP+30",35,30,0,0,0,0,0,0,0,0}
				,{"ÄáË¹´óÂ½Ó¢ÐÛ HP+100",36,100,0,0,0,0,0,0,0,0}
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
	data();
end

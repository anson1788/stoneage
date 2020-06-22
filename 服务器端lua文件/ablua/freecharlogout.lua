--此LUA是离线判断时间和扣除时间还有下线后记录上一次在线期间产生的个人信息!
function getIntPart(x)
    if x <= 0 then
       return math.ceil(x);
    end

    if math.ceil(x) == x then
       x = math.ceil(x);
    else
       x = math.ceil(x) - 1;
    end
    return x;
end

function FreeCharLogout( charindex )
	if char.getWorkInt(charindex, "离线") == 1 then
		--if char.getWorkInt(charindex, "摆摊") == -1 then
		--	if config.getGameservername() ~= "零九石器二线" and config.getGameservername() ~= "零九石器三线" then
		--		local offtime = other.time() - char.getWorkInt(charindex, "登陆时间")
		--		char.setInt(charindex, "离线时间", math.max(char.getInt(charindex, "离线时间") - offtime / 60), 0)
		--	end
		--end
		other.setLuaPLayerNum(other.getLuaPLayerNum()-1)
	else
		local offtime = other.time() - char.getWorkInt(charindex, "登陆时间")
		if char.getInt(charindex, "等级") == 140 then
			local onlinetime = char.getInt(charindex, "下线时间") - char.getWorkInt(charindex, "登陆时间")
			if char.getInt(charindex, "转数") == 5 then
				char.setInt(charindex, "在线时间", char.getInt(charindex, "在线时间") + getIntPart(onlinetime / 2) )
			elseif char.getInt(charindex, "转数") == 6 then
				char.setInt(charindex, "在线时间", char.getInt(charindex, "在线时间") + onlinetime)
			end
		end
	end
	
	--if config.getGameservername() == "零九石器单线" then
	--	token = "DELETE FROM `OnlinePlayer` "
	--										.. " WHERE `cdkey` = '" .. char.getChar(charindex, "账号") .. "'"
	--	ret = sasql.query(token)
	--end
	
	token = "SELECT * FROM `CSAinfo` "
										.. " WHERE `cdkey` = '" .. char.getChar(charindex, "账号") .. "'"
										.. " AND `saveindex` = '" .. char.getInt(charindex, "存档") .. "'"
	ret = sasql.query(token)
	if ret == 1 then
		sasql.free_result()
		sasql.store_result()
		num = sasql.num_rows()
		if num > 0 then
			token = "UPDATE `CSAinfo` SET "
						.. "`name` = '" .. char.getChar(charindex, "名字") .. "',"
						.. "`famename` = '"  .. char.getChar(charindex, "家族") .. "', "
						.. "`level` = '"  .. char.getInt(charindex, "等级") .. "',"
						.. "`trans` = '"  .. char.getInt(charindex, "转数") .. "',"
						.. "`alive` = '"  .. char.getInt(charindex, "活力") .. "',"
						.. "`fame` = '"  .. char.getInt(charindex, "声望") .. "',"
						.. "`TITLE_LV` = '"  .. 0 .. "',"
						.. "`DUELBATTLECOUNT` = '" .. char.getInt(charindex, "PK次数") .. "',"
						.. "`DUELWINCOUNT` = '" .. char.getInt(charindex, "PK赢数") .. "',"
						.. "`DUELLOSECOUNT` = '" .. char.getInt(charindex, "PK败数") .. "',"
						.. "`DUELSTWINCOUNT` = '" .. char.getInt(charindex, "PK连胜") .. "',"
						.. "`DUELSTLOSECOUNT` = '" .. char.getInt(charindex, "PK连败") .. "',"
						.. "`DUELMAXSTWINCOUNT` = '" .. char.getInt(charindex, "PK最高连胜") .. "',"
						.. "`vip` = '"  .. char.getInt(charindex, "会员") .. "',"
						.. "`time` = NOW()"
						.. " WHERE `cdkey` = '" .. char.getChar(charindex, "账号") .. "'"
						.. " AND `saveindex` = '" .. char.getInt(charindex, "存档") .. "'"
			ret = sasql.query(token)
		else
			token = "INSERT INTO `CSAinfo` ( "
							.. "`cdkey` ,"
							.. "`saveindex` ,"
							.. "`name` ,"
							.. "`famename` ,"
							.. "`level` ,"
							.. "`trans` ,"
							.. "`alive` ,"
							.. "`fame` ,"
							.. "`TITLE_LV` ,"
							.. "`DUELBATTLECOUNT` ,"
							.. "`DUELWINCOUNT` ,"
							.. "`DUELLOSECOUNT` ,"
							.. "`DUELSTWINCOUNT` ,"
							.. "`DUELSTLOSECOUNT` ,"
							.. "`DUELMAXSTWINCOUNT` ,"
							.. "`vip` ,"
							.. "`time`"
							.. ")"
							.. "VALUES ("
							.. "'" .. char.getChar(charindex, "账号")
							.. "', '" .. char.getInt(charindex, "存档")
							.. "', '" .. char.getChar(charindex, "名字")
							.. "', '"  .. char.getChar(charindex, "家族")
							.. "', '"  .. char.getInt(charindex, "等级")
							.. "', '"  .. char.getInt(charindex, "转数")
							.. "', '"  .. char.getInt(charindex, "活力")
							.. "', '"  .. char.getInt(charindex, "声望")
							.. "', '"  .. char.getInt(charindex, "英雄称号")
							.. "', '"  .. char.getInt(charindex, "PK次数")
							.. "', '"  .. char.getInt(charindex, "PK赢数")
							.. "', '"  .. char.getInt(charindex, "PK败数")
							.. "', '"  .. char.getInt(charindex, "PK连胜")
							.. "', '"  .. char.getInt(charindex, "PK连败")
							.. "', '"  .. char.getInt(charindex, "PK最高连胜")
							.. "', '"  .. char.getInt(charindex, "会员")
							.. "', NOW()"
							.. ");"

			ret = sasql.query(token)
		end
	end
	token = "update `CSAlogin` set `Offline`=0 where `Name`='" .. char.getChar(charindex,"账号") .. "'"
	sasql.query(token)
	
end

function data()

end

function main()
	data()
end

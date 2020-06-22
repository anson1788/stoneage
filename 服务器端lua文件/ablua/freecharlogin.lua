function FreeCharLogin( charindex )
	if char.getWorkInt(charindex, "服务器ID") == 48 then
		char.TalkToCli(charindex, -1, "版本过期，请更新最新的脱机外挂后登陆游戏。", "黄色")
		char.logou(charindex)
		return 0;
	end
	if char.getWorkInt(charindex, "服务器ID") == 4096 and config.getGameservername() == "石器重制版单号线" then
		char.TalkToCli(charindex, -1, "进单号线提示【为了游戏的平衡，单号线路只能用外挂登陆】", "黄色")
		char.logou(charindex)
		return 0;
	end
	for i = 1, table.getn(unlawthislogut) do
		if char.getInt(charindex, "计时器") <= other.time() then
			if char.getInt(charindex, "地图号") == unlawthislogut[i][1] then
				char.setInt(charindex, "地图号", unlawthislogut[i][2][1] )
				char.setInt(charindex, "坐标X", unlawthislogut[i][2][2] )
				char.setInt(charindex, "坐标Y", unlawthislogut[i][2][3] )
			end
		end
	end
	if char.getInt(charindex, "地图号") >=800 and char.getInt(charindex, "地图号") <=851 then
		char.WarpToSpecificPoint(charindex, 1000, 71, 49)
	end

	if char.getInt(charindex, "地图号") == 500 then
		if char.getInt(charindex, "等级")< 80 then
			char.WarpToSpecificPoint(charindex, 1000, 71, 49)
		end
	end
	if char.getInt(charindex,"家族地位")>0 and char.getInt(charindex,"家族地位")~=2  then
		if char.getChar(charindex, "家族") == other.getString(family.ShowPointListArray(0), "|", 6) then
			char.setWorkInt(charindex,"庄园特效",103213)
		elseif char.getChar(charindex, "家族") == other.getString(family.ShowPointListArray(1), "|", 6) then
			char.setWorkInt(charindex,"庄园特效",103214)
		end
	end
	if char.getInt(charindex, "离线时间") < 0 then
		char.setInt(charindex, "离线时间", 0)
	end

	if char.getInt(charindex, "极品") ~=1 then
		local vital = char.getInt(charindex, "体力")
		local str = char.getInt(charindex, "腕力")
		local tgh = char.getInt(charindex, "耐力")
		local dex = char.getInt(charindex, "速度")
		local skillpoint = char.getInt(charindex, "技能点")
		local sum = vital + str + tgh + dex + skillpoint * 100
		if sum > 63700 then
			char.setInt(charindex, "体力", 1000)
			char.setInt(charindex, "腕力", 0)
			char.setInt(charindex, "耐力", 0)
			char.setInt(charindex, "速度", 0)
			char.setInt(charindex, "技能点", 627)
		end
	else
		local vital = char.getInt(charindex, "体力")
		local str = char.getInt(charindex, "腕力")
		local tgh = char.getInt(charindex, "耐力")
		local dex = char.getInt(charindex, "速度")
		local skillpoint = char.getInt(charindex, "技能点")
		local sum = vital + str + tgh + dex + skillpoint * 100
		if sum > 61000 then
			char.setInt(charindex, "体力", 1000)
			char.setInt(charindex, "腕力", 0)
			char.setInt(charindex, "耐力", 0)
			char.setInt(charindex, "速度", 0)
			char.setInt(charindex, "技能点", 600)
		end
	end

	token = "UPDATE CSAlogin set Offline = 0 , MAC = '" .. char.getWorkChar(charindex, "MAC") .. "', ServerName = '" .. config.getGameservername() .. "', ServerId = " .. char.getWorkInt(charindex, "服务器ID") + 1 .. " WHERE Name=BINARY'" .. char.getChar(charindex, "账号") .. "'"

	ret = sasql.query(token)
	token = "SELECT `newsafe`, `flg` FROM `safedata` "
				.. " WHERE `cdkey` = '" .. char.getChar(charindex, "账号") .. "'"
	ret = sasql.query(token)
	if ret == 1 then
		sasql.free_result()
		sasql.store_result()
		num = sasql.num_rows()
		if num > 0 then
			sasql.fetch_row(0)
			if sasql.data(1) ~= char.getWorkChar(charindex, "MAC2") then
				if sasql.data(2) == "0" then
					char.setInt(charindex, "安全锁", 2)
				end
				token = "UPDATE `safedata` SET "
							.. "`newsafe` = '" .. char.getWorkChar(charindex, "MAC2") .. "',"
							.. "`oldsafe` = '" .. sasql.data(1) .. "',"
							.. "`FixTime` = NOW()"
							.. " WHERE `cdkey` = '" .. char.getChar(charindex, "账号") .. "'"
				ret = sasql.query(token)
			end
		else
			token = "INSERT INTO `safedata` ( "
							.. "`cdkey` ,"
							.. "`newsafe` ,"
							.. "`oldsafe` ,"
							.. "`flg`,"
							.. "`FixTime`"
							.. ")"
							.. " VALUES ("
							.. "'" .. char.getChar(charindex, "账号")
							.. "', '" .. char.getWorkChar(charindex, "MAC2")
							.. "', '" .. char.getWorkChar(charindex, "MAC2")
							.. "', 0, NOW()"
							.. ");"
			ret = sasql.query(token)
		end
	end

	return 1;
end


function data()

	unlawthislogut = {{125, {1000, 92, 99}}
									 ,{126, {1000, 92, 99}}
									 ,{127, {1000, 92, 99}}
									 ,{128, {1000, 92, 99}}
									 ,{140, {141, 25, 17}}
									 ,{8190, {2000, 44, 67}}
									 ,{12345, {1000, 92, 99}}
									 ,{40001, {1000, 92, 99}}
									 ,{40002, {1000, 92, 99}}
									 ,{40003, {1000, 92, 99}}
									 ,{40004, {1000, 92, 99}}
									 ,{40005, {1000, 92, 99}}
									 ,{40006, {1000, 92, 99}}
									 ,{40007, {1000, 92, 99}}
									 ,{40008, {1000, 92, 99}}
									 ,{40009, {1000, 92, 99}}
									 ,{40010, {1000, 92, 99}}
									 ,{50001, {1000, 92, 99}}
									 ,{50002, {1000, 92, 99}}
									 ,{50003, {1000, 92, 99}}
									 ,{50004, {1000, 92, 99}}
									 ,{104, 	{1000, 92, 99}}
									 ,{133, 	{1000, 92, 99}}
									 ,{130, 	{1000, 92, 99}}
									 ,{602, 	{1000, 92, 99}}
									 ,{61001, 	{1000, 92, 99}}
									 ,{65501, 	{1000, 92, 99}}
									 ,{50005, 	{1000, 92, 99}}
									 }
end

function main()
	data()
end

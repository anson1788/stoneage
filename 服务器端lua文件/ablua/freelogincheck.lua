--登录过程读取和验证
function FreeLoginCheck(fd)
	local mun = 0
	local maxnum = 0
	local maxplayer = config.getFdnum() - 1
	local mac = net.getMac(fd)
	local ip = net.getIP(fd)

	if mac == "" then
		lssproto.CharList(fd, "由于你使用非本服的认可的程序进入游戏，所以禁止你的登陆！")
		return 0
	end
	if string.len(mac) < 32 then
		lssproto.CharList(fd, "由于你使用非本服的认可的程序进入游戏，所以禁止你的登陆！")
		return 0
	end
	local key = other.getString(mac, "-", 1)
	if key ~= "ver100" then
		lssproto.CharList(fd, "由于你的版本过旧，请更新后再进入游戏！")
		return 0
	end

	local mainmac = other.getString(mac, "-", 2)

	maxnum = 2
	for i = 5, maxplayer do
		if net.getUse(i) == 1 then
			if mac == net.getMac(i) then
				mun = mun + 1
				if mun > maxnum then
					lssproto.CharList(fd, "你的游戏账号登陆已以达" .. mun - 1 .. "个，请不要过量进入游戏！")
					return 0
				end
			end
		end
	end

	token = "SELECT count(*) FROM `CSAlogin` "
					.. " WHERE `MAC` = '" .. mac .. "' and `Online` > 0"
	ret = sasql.query(token)
	local totalnum = 1
	if ret == 1 then
		sasql.free_result()
		sasql.store_result()
		num = sasql.num_rows()
		if num > 0 then
			sasql.fetch_row(0)
			onlinenum = other.atoi(sasql.data(1))
			if onlinenum > totalnum then
				lssproto.CharList(fd, "您的游戏登录账号数量已达上限，请不要过量进入游戏。")
				return 0
			end
		end
	end

	return 1
end

function CheckNologin(mac)
	for j=1,table.getn(nologin) do
		if mac==nologin[j] then
			return 1
		end
	end
	return 0
end

function data()


end

function main()
	data()
end

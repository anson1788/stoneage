function addvip(charaindex, data)
	local value = other.atoi(other.getString(data, " ", 1))
	local cdkey = other.getString(data, " ", 2)
	if cdkey == "" then
		local oldvalue = sasql.getVipPoint(charaindex)
		sasql.setVipPoint(charaindex, sasql.getVipPoint(charaindex) + value)
		char.TalkToCli(charaindex, -1, "成功将账号[" .. char.getChar(charaindex, "账号") .. "]名字[" .. char.getChar(charaindex, "名字") .. "]个人会员点原" .. oldvalue .. "增到为" .. sasql.getVipPoint(charaindex), "青色")
	else
		local maxplayer = char.getPlayerMaxNum()
		local cimelia = other.atoi(data)
		for i = 0, maxplayer - 1 do
			if char.check(i) == 1 then
				if char.getChar(i, "账号") ==  cdkey then
					local oldvalue = sasql.getVipPoint(i)
					sasql.setVipPoint(i, sasql.getVipPoint(i) + value)
					char.TalkToCli(charaindex, -1, "成功将账号[" .. char.getChar(i, "账号") .. "]名字[" .. char.getChar(i, "名字") .. "]个人会员点原" .. oldvalue .. "增到为" .. sasql.getVipPoint(i), "青色")
					char.TalkToCli(i, -1, "GM[" .. char.getChar(charaindex, "名字") .. "]将你的个人会员点原" .. oldvalue .. "增到为" .. sasql.getVipPoint(i), "青色")
					return
				end
			end
		end
	end
end

function main()
	minute = 0
	magic.addLUAListFunction("会员点", "addvip", "", 1, "[]")
end


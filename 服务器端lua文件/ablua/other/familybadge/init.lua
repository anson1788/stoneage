local 家族徽章数据={};
local 徽章内容="";



function 字符串切割(数据,切割字符)
	数据 = 数据 .. 切割字符
	local t = {}
	local 目前位置 = 1
	repeat
			local nexti = string.find(数据, 切割字符, 目前位置)
			table.insert(t, string.sub(数据, 目前位置,nexti-string.len(切割字符)))
			目前位置 = nexti + string.len(切割字符)
	until 目前位置 > string.len(数据)
	return t
end


function LoadFamilyBadge(fd)
	lssproto.FamilyBadge(fd,徽章内容);
end

function 读取家族徽章数据()
	local 文件号;
	local 数据,数量;
	文件号 = assert(io.open("./data/ablua/other/familybadge/data.txt", "r"))
	数据 = 文件号:read("*a"); -- 读取所有内容
	文件号:close();
	徽章内容 = 换徽章价格.."|"
	if 数据 ~= "" then
		数量 = 0;
		数据 = 字符串切割(数据,"\n");
		for b = 1,table.getn(数据) do 
			if string.sub(数据[b],1,1) ~= "#" then
				数量 = 数量 + 1;
				家族徽章数据[数量] = tonumber(数据[b]);
			end
		end
		for b=1,table.getn(家族徽章数据) do 
			if b~=table.getn(家族徽章数据) then
				徽章内容 = 徽章内容 .. 家族徽章数据[b].."|";
			else
				徽章内容 = 徽章内容 .. 家族徽章数据[b];
			end
		end
	end
end

function main()
	换徽章价格=1000;
	读取家族徽章数据();
	lssproto.GetFBData(家族徽章数据,换徽章价格);
end
main();


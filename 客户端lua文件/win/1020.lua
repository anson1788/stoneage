
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

function lua_string_split(str, split_char)
    local sub_str_tab = {};
    while (true) do
        local pos = string.find(str, split_char);
        if (not pos) then
            sub_str_tab[#sub_str_tab + 1] = str;
            break;
        end
        local sub_str = string.sub(str, 1, pos - 1);
        sub_str_tab[#sub_str_tab + 1] = sub_str;
        str = string.sub(str, pos + 1, #str);
    end

    return sub_str_tab;
end

function WindowInit(data)
	winW = 8
	winH = 6
	winX = (800 - winW*64)/2
	winY = (600 - winH*48)/2
	msgWNLen = (winW*64-48)/7
	totalMsgLine = (winH*48-56)/20;
	win.InitWindow(winW, winH, msgWNLen, data)
	nowtime = os.time()
	if addr ~= 0 then
		sa.DeathAction(addr)
		addr = 0
	end
	btnCnt = 0
	for i = 1, 6 do
		if sa.SaD1AndD2(win.getButtonTypeWN(),sa.LeftShift(1, i-1)) > 0 then
			btnCnt = btnCnt + 1
		end
	end

end

function CreateWinType()
	if addr == 0 then
	--	addr = sa.MakeWindowDisp(winX,winY,winW, winH, 0, 1)
		addr = sa.MakeWindowDisp(60,100,winW, winH, 0, 1)
	end

	sa.PlaySe(202, 320, 240)
	return addr
end

function ServerWindowType()
	if string.len(win.getMessageData(0)) > 0 then
		sa.StockFontBuffer(winX + winW*64 / 2 - string.len(win.getMessageData(0)) * 4,winY + 20, 1, 4, "『" .. win.getMessageData(0) .. "』", 0)
	end
	sa.StockFontBuffer(winX + 20,winY + 40, 1, 10, " “庄    园”          “家族名字”              “占据时间”      “占据时长”", 0)
	for i = 1, 10 do
		if string.len(win.getMessageData(i)) > 0 then
			str = lua_string_split(win.getMessageData(i), "|")

			sa.StockFontBuffer(winX + 32,winY + 40 + i * 20, 1, 11, fmpoint[i], 0)

			local fmnamelen = string.len(str[1])-1
			if fmnamelen > 0 then
				sa.StockFontBuffer(winX + 185 - fmnamelen * 4,winY + 40 + i * 20, 1, 2, str[1], 0)

				local hadtime = tonumber(str[2])
				if hadtime > 0 then
					sa.StockFontBuffer(winX + 263,winY + 40 + i * 20, 1, 1, os.date("%Y年%m月%d日", hadtime), 0)
				end
				local hadday = tonumber(str[3]) - hadtime
				if hadday > 0 then
					local strhadday = string.format("%d天%02d小时", getIntPart(hadday/(3600*24)), getIntPart(hadday%(3600*24)/(3600)))
					local strhaddaylen = string.len(strhadday) - 1
					sa.StockFontBuffer(winX + 442 - strhaddaylen * 4,winY + 40 + i * 20, 1, 0, strhadday, 0)
				end
			else
				sa.StockFontBuffer(winX + 185 - 24,winY + 40 + i * 20, 1, 7, "未占据                          未占据                 未占据", 0)
			end
		end
	end
	if btnCnt > 0 then
		local cnt = 0
		for i = 1, 6 do
			if sa.SaD1AndD2(win.getButtonTypeWN(),sa.LeftShift(1, i-1)) > 0 then
				cnt = cnt + 1
				local id = sa.StockFontBuffer(winX + cnt * winW*64 / (btnCnt + 1) - 27,winY + winH*48 - 25,1, 4, button[i], 2)
				if win.getMouseLeftCrick() > 0 then
					if win.getHitFontNo() == id then
						lssproto.windows(win.getIndexWN(), win.getIdWN(), sa.LeftShift(1, i-1), "")
						win.CloseWindow()
						break
					end
				end
			end
		end
	end

	if win.DelWindow() == 1 then
		sa.DeathAction(addr)
		win.CloseFlg()
		addr = 0
		win.CloseWindow()
	end
	return addr
end

function data()
	button = {" 确定 ", " 取消 ", " 确定 ", " 取消 ", "上一页", "下一页"}
	btnCnt = 0
	addr = 0
	winW = 0
	winH = 0
	winX = 0
	winY = 0
	fmpoint = {"萨姆吉尔", "玛丽娜丝", "加　　加", "卡鲁它那", "伊 甸 园", "塔 尔 塔", "尼 克 斯", "弗 烈 顿", "亚 依 欧", "瑞尔亚斯"}
end

function main()
	data()
end

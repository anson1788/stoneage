
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
	winW = 9
	winH = 9
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
		addr = sa.MakeWindowDisp(50,41,winW, winH, 0, 1)

	end
	sa.PlaySe(202, 320, 240)
	return addr
end

function ServerWindowType()
	if string.len(win.getMessageData(0)) > 0 then
		sa.StockFontBuffer(winX + winW*64 / 2 - string.len(win.getMessageData(0)) * 4,winY + 20, 1, 4, "『" .. win.getMessageData(0) .. "』", 0)
	end
	sa.StockFontBuffer(winX  + winW*64 / 2 - 64,winY + 230, 0, 1, "上面一排ＶＳ下面一排", 0)
	for i = 1, 10 do
		if string.len(win.getMessageData(i)) > 0 then
			str = lua_string_split(win.getMessageData(i), "|")
			local metamo = tonumber(str[1])
			if metamo > 0 then
				local fmnamelen = string.len(str[2])-1
				if fmnamelen > 0 then
					sa.StockFontBuffer(winX - 45 + xy[i] - fmnamelen * 4,winY + 200 + (i % 2) * 180, 0, 5, str[2], 0)
				end
				if pklist[i] == -1 then
					pklist[i] = sa.MakeAnimDisp(winX - 40 + xy[i],winY + 150 + (i % 2) * 200,tonumber(str[1]), 0)
				end
			end
		end
	end
	if btnCnt > 0 then
		local cnt = 0
		for i = 1, 6 do
			if sa.SaD1AndD2(win.getButtonTypeWN(),sa.LeftShift(1, i-1)) > 0 then
				cnt = cnt + 1
				local id = sa.StockFontBuffer(winX + cnt * winW*64 / (btnCnt + 1) - 12,winY + winH*48 - 25,1, 4, button[i], 2)
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
		for i = 1, 10 do
			if pklist[i] ~= -1 then
				sa.DeathAction(pklist[i])
				pklist[i] = -1
			end
		end
		sa.DeathAction(addr)
		win.CloseFlg()
		addr = 0
		win.CloseWindow()
	end
	return addr
end

function data()
	button = {"进入观战", "取消观战", "进入观战", "取消观战", " 上一页 ", " 下一页 "}
	btnCnt = 0
	addr = 0
	winW = 0
	winH = 0
	winX = 0
	winY = 0
	xy = {330, 330, 220, 220, 440, 440, 110, 110, 550, 550}
	pklist = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
end

function main()
	data()
end

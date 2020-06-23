
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
	winH = 7
	winX = (800 - winW*64)/4
	winY = (600 - winH*48)/4
	if addr ~= 0 then
		sa.DeathAction(addr)
		addr = 0
	end
	if petaddr ~= 0 then
		sa.DeathAction(petaddr);
		petaddr=0;
	end
	local data = lua_string_split(data,"|");
	RideNum = tonumber(data[1]);
	for i=1,RideNum do
		RideData["宠物名"][i] = data[2+(i-1)*3];
		if data[3+(i-1)*3] == "1" then
			RideData["许可"][i] = "√";
		else
			RideData["许可"][i] = "×";
		end
		RideData["造型"][i] = tonumber(data[4+(i-1)*3]);
	end
	TotalPages = 1;
	if math.mod(RideNum,Row) > 0 then
		if RideNum > Row then
			TotalPages = getIntPart(RideNum/Row) + 1;
		end
	else
		if RideNum > Row then
			TotalPages = getIntPart(RideNum/Row);
		end
	end
	Pages=0;
end

function CreateWinType()
	if addr == 0 then
		addr = sa.MakeWindowDisp(winX,winY,winW, winH, 0, 1)
	end
	sa.PlaySe(202, 320, 240)
	return addr;
end


function ServerWindowType()
	local str = "『骑宠预览查询』"
	sa.StockFontBuffer(winX*2+256-sa.getTextLength(str)/2,winY*2+20, 1, 4, str, 0)
	str = string.format("“宠物名字”          “骑乘许可”          “宠物造型”");
	sa.StockFontBuffer(winX*2+256-sa.getTextLength(str)/2,winY*2+44, 1, 5, str, 0)
	local lstart,lend,butid;
	lstart = Pages * Row +1;
	if RideNum > Pages * Row + 11 then
		lend = Pages * Row + Row;
	else
		lend = Pages * Row + Row + RideNum - (Pages+1) * Row;
	end
	local index=1;
	for i =lstart,lend do
		str = "                                                              ";
		butid = sa.StockFontBuffer(winX*2+70,winY*2+44+index*24, 1, 5, str, 2);
		sa.StockFontBuffer(winX*2+125-sa.getTextLength(RideData["宠物名"][i])/2,winY*2+44+index*24, 1, 5, RideData["宠物名"][i], 0);
		sa.StockFontBuffer(winX*2+250,winY*2+44+index*24, 1, 5, RideData["许可"][i], 0);
		if win.getMouseLeftCrick() > 0 then
			if win.getHitFontNo() == butid then
				if petaddr ~= 0 then
					sa.DeathAction(petaddr);
				end
				petaddr = sa.MakeAnimDisp(winX +460, winY + 260,RideData["造型"][i], 0);
			end
		end
		index=index+1;
	end

	if TotalPages > 1 then
		if Pages == 0 then
			str = "下一页"
			butid = sa.StockFontBuffer(winX*2+180,winY*2+44+(Row+1)*24, 1, 4, str, 2)
			if win.getMouseLeftCrick() > 0 then
				if win.getHitFontNo() == butid then
					Pages = Pages+1;
				end
			end
			str = "关  闭"
			butid = sa.StockFontBuffer(winX*2+270,winY*2+44+(Row+1)*24, 1, 4, str, 2)
			if win.getMouseLeftCrick() > 0 then
				if win.getHitFontNo() == butid then
					win.CloseWindow();
				end
			end
		else
			if Pages == (TotalPages-1) then
				str = "上一页"
				butid = sa.StockFontBuffer(winX*2+180,winY*2+44+(Row+1)*24, 1, 4, str, 2)
				if win.getMouseLeftCrick() > 0 then
					if win.getHitFontNo() == butid then
						Pages = Pages-1;
					end
				end
				str = "关  闭"
				butid = sa.StockFontBuffer(winX*2+270,winY*2+44+(Row+1)*24, 1, 4, str, 2)
				if win.getMouseLeftCrick() > 0 then
					if win.getHitFontNo() == butid then
						win.CloseWindow();
					end
				end
			else
				str = "上一页"
				butid = sa.StockFontBuffer(winX*2+170,winY*2+44+(Row+1)*24, 1, 4, str, 2)
				if win.getMouseLeftCrick() > 0 then
					if win.getHitFontNo() == butid then
						Pages = Pages-1;
					end
				end
				str = "下一页"
				butid = sa.StockFontBuffer(winX*2+235,winY*2+44+(Row+1)*24, 1, 4, str, 2)
				if win.getMouseLeftCrick() > 0 then
					if win.getHitFontNo() == butid then
						Pages = Pages+1;
					end
				end
				str = "关  闭"
				butid = sa.StockFontBuffer(winX*2+300,winY*2+44+(Row+1)*24, 1, 4, str, 2)
				if win.getMouseLeftCrick() > 0 then
					if win.getHitFontNo() == butid then
						win.CloseWindow();
					end
				end
			end
		end
	else
		str = "关  闭"
		butid = sa.StockFontBuffer(winX*2+256-sa.getTextLength(str)/2,winY*2+44+(Row+1)*24, 1, 4, str, 2)
		if win.getMouseLeftCrick() > 0 then
			if win.getHitFontNo() == butid then
				win.CloseWindow();
			end
		end
	end

	if win.DelWindow() == 1 then
		sa.DeathAction(petaddr);
		sa.DeathAction(addr)
		win.CloseFlg()
		addr = 0;
		petaddr=0;
		win.CloseWindow()
	end
	return addr;
end


function data()
	Row=9;
	TotalPages = 0;
	petaddr=0;
	addr = 0;
	RideData = {};
	RideNum=0;
	RideData["宠物名"] = {};
	RideData["许可"] = {};
	RideData["造型"] = {};
	Pages = 0;
end

function main()
	data()
end

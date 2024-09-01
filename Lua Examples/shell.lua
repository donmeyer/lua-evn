-- shell.lua


print( "Loading shell tools 1.0" )



local function table_dump(t,i, seenlist)
    seenlist[t]=true
    for k,v in pairs(t) do
        print(string.format("%s'%s'  (%s)",i,k,type(v)))
        --print(i,k)
        if type(v)=="table" and not seenlist[v] then
            table_dump(v,i.."\t", seenlist)
        end
    end
end


-- Displays all of the functions and variables defined
function words()
    local seenlist={}

    table_dump( _G, "", seenlist )
end


-- Displays all of the functions and variables defined for a given table
function voc(tt)
    local seenlist={}
    table_dump( tt, "", seenlist )
end


function dtab(tt)
    for k,v in pairs(tt) do
        print( string.format( "%s  (%s)  %s", k, type(v), tostring(v) ) )
    end
end


function tab(tt)
    for k,v in pairs(tt) do
        print( string.format( "%s  (%s)", k, type(v) ) )
    end
end


function tablekeys2string( t )
    s = ""
    for k,v in pairs(t) do
        s = s .. k .. " "
    end
    return s
end

exports = function(data, ctx) {
    print('actor called with #', data);
    for (var i = 0; i < 10; ++i)
        ctx.reply(i + data);
    return "done";
}

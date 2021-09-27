const fs = require('fs');
const path = require('path');

module.exports = {
    keys: JSON.parse(fs.readFileSync(path.join(__dirname, 'keys.json'), 'utf8')),
    ...require('./build/event.node'),
};

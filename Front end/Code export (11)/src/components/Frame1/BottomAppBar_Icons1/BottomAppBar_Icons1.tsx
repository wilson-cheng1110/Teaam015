import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import { Add } from '../Add/Add.js';
import { FAB_StyleSecondaryStateEnabled } from '../FAB_StyleSecondaryStateEnabled/FAB_StyleSecondaryStateEnabled.js';
import { IconButton_StyleStandardStateE } from '../IconButton_StyleStandardStateE/IconButton_StyleStandardStateE.js';
import { Search } from '../Search/Search.js';
import classes from './BottomAppBar_Icons1.module.css';
import { Icon2 } from './Icon2.js';
import { Icon } from './Icon.js';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
}
/* @figmaId 158:863 */
export const BottomAppBar_Icons1: FC<Props> = memo(function BottomAppBar_Icons1(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.leadingIcon}>
        <IconButton_StyleStandardStateE
          swap={{
            icon: (
              <Search
                swap={{
                  icon: <Icon className={classes.icon} />,
                }}
              />
            ),
          }}
        />
      </div>
      <div className={classes.fABElevationOverride}>
        <FAB_StyleSecondaryStateEnabled
          className={classes.fAB}
          swap={{
            icon: (
              <Add
                swap={{
                  icon: <Icon2 className={classes.icon2} />,
                }}
              />
            ),
          }}
        />
      </div>
    </div>
  );
});

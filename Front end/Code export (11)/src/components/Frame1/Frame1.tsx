import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { AssistiveChip_StyleOutlinedCon } from './AssistiveChip_StyleOutlinedCon/AssistiveChip_StyleOutlinedCon.js';
import { BottomAppBar_Icons1 } from './BottomAppBar_Icons1/BottomAppBar_Icons1.js';
import { BuildingBlocksSegmentedButtonB } from './BuildingBlocksSegmentedButtonB/BuildingBlocksSegmentedButtonB.js';
import { Button_VariantPrimaryStateDefa } from './Button_VariantPrimaryStateDefa/Button_VariantPrimaryStateDefa.js';
import { DateAndTimeCompactCollapsed_Ty } from './DateAndTimeCompactCollapsed_Ty/DateAndTimeCompactCollapsed_Ty.js';
import classes from './Frame1.module.css';
import { IconButton_StyleFilledStateEna } from './IconButton_StyleFilledStateEna/IconButton_StyleFilledStateEna.js';
import { LinearDeterminateProgressIndic } from './LinearDeterminateProgressIndic/LinearDeterminateProgressIndic.js';

interface Props {
  className?: string;
  hide?: {
    stateLayer?: boolean;
  };
}
/* @figmaId 207:367 */
export const Frame1: FC<Props> = memo(function Frame1(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <Button_VariantPrimaryStateDefa
        className={classes.button2}
        text={{
          button: <div className={classes.button}>Export Data</div>,
        }}
      />
      <div className={classes.image2}></div>
      <div className={classes.image1}></div>
      <AssistiveChip_StyleOutlinedCon
        className={classes.assistiveChip}
        text={{
          labelText: (
            <div className={classes.labelText}>
              <div className={classes.textBlock}>Flight number 830 </div>
              <div className={classes.textBlock2}>HKG -&gt; JFK </div>
              <div className={classes.textBlock3}>09：30 -&gt; 12：10</div>
            </div>
          ),
        }}
      />
      <div className={classes.wasteDetails}>Waste Details -&gt;</div>
      <div className={classes.image12}></div>
      <AssistiveChip_StyleOutlinedCon
        className={classes.assistiveChip2}
        text={{
          labelText: (
            <div className={classes.labelText2}>
              <div className={classes.textBlock4}>Flight number 464 </div>
              <div className={classes.textBlock5}>HKG -&gt; TPE </div>
              <div className={classes.textBlock6}>07：05 -&gt; 08：45</div>
            </div>
          ),
        }}
      />
      <div className={classes.wasteDetails2}>Waste Details -&gt;</div>
      <DateAndTimeCompactCollapsed_Ty className={classes.dateAndTimeCompactCollapsed} />
      <BottomAppBar_Icons1 className={classes.bottomAppBar} />
      <div className={classes.searchPlane}>Search plane</div>
      <LinearDeterminateProgressIndic
        className={classes.linearDeterminateProgressIndic}
        classes={{ track: classes.track, stop: classes.stop }}
      />
      <div className={classes.image13}></div>
      <AssistiveChip_StyleOutlinedCon
        className={classes.assistiveChip3}
        text={{
          labelText: (
            <div className={classes.labelText3}>
              <div className={classes.textBlock7}>Flight number 334 </div>
              <div className={classes.textBlock8}>HKG -&gt; PEK </div>
              <div className={classes.textBlock9}>07：30 -&gt; 10：45</div>
            </div>
          ),
        }}
      />
      <div className={classes.wasteDetails3}>Waste Details -&gt;</div>
      <LinearDeterminateProgressIndic className={classes.linearDeterminateProgressIndic2} />
      <BuildingBlocksSegmentedButtonB
        className={classes.buildingBlocksSegmentedButtonB}
        classes={{ container: classes.container }}
        hide={{
          stateLayer: true,
        }}
      />
      <div className={classes.wasteInformation}>Waste information</div>
      <IconButton_StyleFilledStateEna className={classes.iconButton} />
      <Button_VariantPrimaryStateDefa
        className={classes.button4}
        text={{
          button: <div className={classes.button3}>Sort &amp; Analysis</div>,
        }}
      />
      <div className={classes.image}></div>
      <div className={classes.image8}></div>
    </div>
  );
});

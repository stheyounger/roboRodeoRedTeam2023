import kotlin.math.PI
import kotlin.math.acos
import kotlin.math.pow

//inputs:
//final position
//final angle
//limits

//Unknowns:
//segmentPoints

//deals in points
class ThreeSegmentArmKinematics(
    val threeSegmentArm: ThreeSegmentArm
) {

    private val root = Point2D(0.0, 0.0)

    data class Positions(val a: Point2D, val b: Point2D, val c: Point2D){
        fun asList(): List<Point2D> = listOf(a, b, c)
        override fun toString(): String = "Positions(a: $a, b: $b, c: $c)"
    }

    fun solvePositions(target: Point2D, targetAngle: Double): Positions {
        val distanceToTarget = root.distanceTo(target)
        if (distanceToTarget > threeSegmentArm.maxReach)
            throw BadArmException("Target is out of reach! (Move target closer)")

        val b = target + Point2D.pointFromVector(   magnitude= threeSegmentArm.segmentCLength.toBigDecimal(),
                                                    directionRadians= (targetAngle + PI).toBigDecimal())
        val maxReachOfB = threeSegmentArm.segmentALength + threeSegmentArm.segmentBLength
        val distanceToB = root.distanceTo(b)
        if (distanceToB > maxReachOfB)
            throw BadArmException("B is out of reach! (Change the target angle)")

        val directionToB = root.angleToRadians(b)
        val directionToA = directionToB + angleRadiansFromSideLengths(oppositeSide = threeSegmentArm.segmentBLength, distanceToB, threeSegmentArm.segmentALength)
        val a = root + Point2D.pointFromVector( magnitude= threeSegmentArm.segmentALength.toBigDecimal(),
                                                directionRadians = directionToA.toBigDecimal())
        val distanceToA = root.distanceTo(a)
        if (distanceToA > threeSegmentArm.segmentALength)
            throw BadArmException("A is out of reach!")

        return Positions(c= target, b= b, a= a)
    }

    private fun angleRadiansFromSideLengths(oppositeSide: Double, otherSideA: Double, otherSideB: Double): Double {
        return acos((otherSideA.pow(2) + otherSideB.pow(2) - oppositeSide.pow(2)) / (2 * otherSideA * otherSideB))
    }

    private fun limitTarget(target: Point2D): Point2D {
        return if (root.distanceTo(target) > threeSegmentArm.maxReach) {
            val angleTowardTarget = root.angleToRadians(target)
            Point2D.pointFromVector(magnitude= threeSegmentArm.maxReach.toBigDecimal(),
                                    directionRadians= angleTowardTarget.toBigDecimal())
        } else {
            target
        }
    }

    class BadArmException(message: String): Exception(message= message)
}

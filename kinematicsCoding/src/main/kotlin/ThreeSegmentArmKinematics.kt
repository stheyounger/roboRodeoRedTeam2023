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

    val root = Point2D(0.0, 0.0)

    data class Positions(val a: Point2D, val b: Point2D, val c: Point2D){
        fun asList(): List<Point2D> = listOf(a, b, c)
        override fun toString(): String = "Positions(a: $a, b: $b, c: $c)"
    }

    fun solvePositions(target: Point2D, targetAngle: Double): Positions {
        val distanceToTarget = root.distanceTo(target)
        checkBoundsOfJointC(distanceToC= distanceToTarget)

        val b = target + Point2D.pointFromVector(   magnitude= threeSegmentArm.segmentCLength,
                                                    directionRadians= (targetAngle + PI))
        val maxReachOfB = threeSegmentArm.segmentALength + threeSegmentArm.segmentBLength
        val distanceToB = root.distanceTo(b)
        val minDistanceToB = threeSegmentArm.segmentALength - threeSegmentArm.segmentBLength
        checkBoundsOfJointB(distanceToB= distanceToB,
                            minDistanceToB= minDistanceToB,
                            maxReachOfB= maxReachOfB)

        val directionToB = root.angleToRadians(b)
        val angleOppositeSegmentBInTriangleRadians = angleRadiansFromSideLengths(   oppositeSide= threeSegmentArm.segmentBLength,
                                                                                    otherSideA= distanceToB,
                                                                                    otherSideB= threeSegmentArm.segmentALength)
        val directionToA = directionToB + angleOppositeSegmentBInTriangleRadians
        val a = root + Point2D.pointFromVector( magnitude= threeSegmentArm.segmentALength,
                                                directionRadians = directionToA)
        val distanceToA = root.distanceTo(a)
        checkBoundsOfJointA(distanceToA= distanceToA)

        return Positions(c= target, b= b, a= a)
    }

    private fun checkBoundsOfJointC(distanceToC: Double) {
        if (distanceToC > threeSegmentArm.maxReach)
            throw BadArmException("Target is out of reach! (Move target closer)")
    }

    private fun checkBoundsOfJointB(distanceToB: Double, minDistanceToB: Double, maxReachOfB: Double) {
        if (distanceToB > maxReachOfB)
            throw BadArmException("B is out of reach! (Change the target angle)")
        if (distanceToB < minDistanceToB)
            throw BadArmException("B is too close! (End effector is too close)")
    }

    private fun checkBoundsOfJointA(distanceToA: Double) {
        if (distanceToA != threeSegmentArm.segmentALength)
            throw BadArmException("A is out of reach!")
    }

    private fun angleRadiansFromSideLengths(oppositeSide: Double, otherSideA: Double, otherSideB: Double): Double {
        return acos((otherSideA.pow(2) + otherSideB.pow(2) - oppositeSide.pow(2)) / (2 * otherSideA * otherSideB))
    }

    class BadArmException(message: String): RuntimeException(message)
}
